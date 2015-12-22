#include "MemWorker.hpp"
#include "Parser.hpp"
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <sstream>
#include <string>
#include <iostream>
#include <Eigen/Dense>

#define DELIMITER  ','
#define NEw_LINE 10


// mmap + eigen
namespace avant_analytics
{

	map<int, list<OP> > MemWorker::read(string rule_dir)
	{
		ofstream out;
		out.open(rule_dir + ".out", ofstream::out);
		ifstream rule_file;
		rule_file.open(rule_dir, ifstream::in);
		map<int, list<OP> > rules;
		vector<int> cols;
		cols.resize(col_size);
		init_cols(cols, 1);
		string line;
		while ( getline(rule_file, line) )
		{	
			vector<string> splitted;
			boost::split(splitted, line, boost::is_any_of(":"));
			Parser::parse(splitted, cols, rules);
		}
		rule_file.close();
		return rules;
	}

	static void MemWorker::check (int test, const char * message, ...)
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


	//mmap
	size_t MemWorker::load_data(string data_file, char * mapped)
	{
		int fd;
		struct stat s;
		int status;
		size_t size;
		//const char * mapped;

		fd = open (data_file, O_RDONLY);
		check (fd < 0, "open %s failed: %s", data_file, strerror (errno));

		/* Get the size of the file. */
		status = fstat (fd, & s);
		check (status < 0, "stat %s failed: %s", data_file, strerror (errno));
		size = s.st_size;

		/* Memory-map the file shared. */
		mapped = (char*) mmap (0, size, PROT_READ, MAP_SHARED, fd, 0);
		check (mapped == MAP_FAILED, "mmap %s failed: %s",
				data_file, strerror (errno));
		return size;
	}

	// load into eigen
	// different logic for matrix computation
	void MemWorker::process(string rule_dir, int col_size, int row_size, string data_file, map<int, FUNC>& func_map)
	{
		ofstream out;
		out.open(rule_dir + ".out", ofstream::out);
		map<int, list<OP> > rules = read(rule_dir);
		char * data;
		size_t file_size = load_data(data_file, data);
		Matrix<float, row_size, col_size> matrix;
		string value;
		int count = 0;
		int new_col = 0;
		map<int, int> col_mapping;
		if ( data != null )
		{
			for (int i = 0; i < file_size; ++i )
			{
				char c;
				c = mapped[i];
				if ( c != DELIMITER && c != NEW_LINE )
				{
					value.append(1, c);
				}else
				{
					int row = count / col_size;
					int col = count - (row * col_size);
					map<int, list<OP> >::iterator map_it = rules.find(stoi(col));
					if (map_it == rules.end()) continue;
					matrix(row,new_col) =  stof(value);
					value.clear();
					count ++;
					new_col++;
					col_mapping[new_col] = col;
				}
			}
		}

		//apply FUNC
		for (int i = 0;  i< new_col; ++i)
		{

					list<OP> list_of_rules = (list<OP>) rules[col_mapping[i]];
					// col-wise operation
					for (list<OP>::iterator it = list_of_rules.begin(); it != list_of_rules.end(); ++it )
					{
						matrix.col[i] = func_map[(*it).op_id]((*it).params, matrix.col[i]);
					}	
			}

		}
		data.close();
		// export matrix to file
		IOFormat CommaInitFmt(StreamPrecision, DontAlignCols, ", ", ", ", "", "", " << ", ";");
		out << matrix.format(CommaInitFmt) << endl;
		out.close();
	}

}
