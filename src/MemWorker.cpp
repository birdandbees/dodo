#include "MemWorker.hpp"

// mmap + eigen
namespace avant_analytics
{
	void MemWorker::check (int test, const char * message, ...)
		{
			if (test) {
				va_list args;
				va_start (args, message);
				vfprintf (stderr, message, args);
				va_end (args);
				fprintf (stderr, "\n");
				exit (EXIT_FAILURE);
			}
		}

	//TODO: mmap, when to unmap???
	char *  MemWorker::load_data(string data_file, size_t& size)
	{
		int fd;
		struct stat s;
		int status;

		fd = open (data_file.c_str(), O_RDONLY);
		check (fd < 0, "open %s failed: %s", data_file.c_str(), strerror (errno));

		/* Get the size of the file. */
		status = fstat (fd, & s);
		check (status < 0, "stat %s failed: %s", data_file.c_str(), strerror (errno));
		size = s.st_size;

		/* Memory-map the file shared. */
		char * mapped = (char*) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);
		check (mapped == MAP_FAILED, "mmap %s failed: %s", data_file.c_str(), strerror (errno));
		return mapped;
	}

	// load into eigen
	// different logic for matrix computation
	// TODO: exception handling
	void MemWorker::process(string rule_dir, string col_file, int col_size, int row_size, string data_file)
	{
		ofstream out;
		out.open(col_file + ".out", ofstream::out);
		vector<int> cols;
	  map<int, list<OP> > col_rules;
	 	map<int, list<OP> > row_rules;
		list<DepRule> dep_rules;
		load_cols(col_file, cols);
		map<int, MFUNC> mfunc_map;
		map<int, FUNC> func_map;
		MungeBits::load_funcs(func_map);
		MungeBits::load_funcs(mfunc_map);
		read(rule_dir, cols, col_rules, row_rules, dep_rules);
		char * data;
		size_t file_size;
		data = load_data(data_file, file_size);
		Matrix<float, Dynamic, Dynamic> matrix(row_size, cols.size());;
		string value;
		int count = 0;
		int new_count = 0;
		int new_row = 0;
		map<int, int> col_mapping;
		map<int, int> col_reverse_mapping;
		//apply row-level rules when loading into eigen
		if ( data != NULL )
		{
			for (size_t i = 0; i < file_size; ++i )
			{
				char c;
				c = data[i];
				if ( c != DELIMITER && c != NEW_LINE )
				{
					value.append(1, c);
				}else
				{

					int row = count / col_size;
					int col = count - (row * col_size);
					float this_value = stof(value);
					value.clear();
					count ++;
					// only consider this partiton relevant cols
					if ( !binary_search(cols.begin(), cols.end(), col) ) continue;
					map<int, list<OP> >::iterator map_it = row_rules.find(row);
					if (map_it != row_rules.end())
					{
						//drop rows
						bool keep = true;
						for(auto i : map_it->second)
						{
							if (i.op_id == 0 ) { keep = false; continue;};
							// TODO: filter rules params
						}
						if (!keep) continue;
					};	
					new_row = new_count / cols.size();
					int new_col = new_count - (new_row * cols.size());
					new_count++;
					matrix(new_row,new_col) =  this_value;
					//cout << "[" << new_row << "][" << new_col << "] =" << this_value<< endl;
					col_mapping[new_col] = col;
					col_reverse_mapping[col] = new_col;
				}
			}
		}
		new_row ++;
		matrix.conservativeResize(new_row,cols.size());

		//TODO: apply dep_rules
		typedef Matrix<float, Dynamic, 1> COLUMN;
		for(auto rule : dep_rules)
		{
			  vector< Ref<COLUMN> > operands;
				for (auto i : rule.range )
				{
					 operands.push_back(matrix.col(col_reverse_mapping[i]));
				}
				mfunc_map[rule.op_id](rule.params, operands);
		};

		//apply col-level FUNC
		for (size_t i = 0;  i< cols.size(); ++i)
		{

			list<OP> list_of_rules = (list<OP>) col_rules[col_mapping[i]];
			// col-wise operation
			for (list<OP>::iterator it = list_of_rules.begin(); it != list_of_rules.end(); ++it )
			{
				vector < Ref<COLUMN> > operator_vec;
				operator_vec.push_back(matrix.col(i));
				mfunc_map[(*it).op_id]((*it).params, operator_vec);
			}	
		}


		//data.close();
		// export matrix to file
		//IOFormat CommaInitFmt(StreamPrecision, DontAlignCols, ", ", ", ", "", "", " << ", ";");
		//out << matrix.format(CommaInitFmt) << endl;
		out << matrix << endl;
		out.close();
	}

}
