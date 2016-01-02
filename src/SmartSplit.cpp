#include "SmartSplit.hpp"

namespace avant_analytics
{
	template <typename Rank, typename Parent>
		int SmartSplit::partition(Rank& r, Parent& p, vector<int>& elements, queue<dep_pair>& pair_queue)
		{
			boost::disjoint_sets<Rank,Parent> dsets(r, p);
			for (vector<int>::iterator e = elements.begin(); e != elements.end(); e++)
				dsets.make_set(*e);
			while ( !pair_queue.empty() ) { 
				dep_pair e = pair_queue.front();
				pair_queue.pop();
				int u = dsets.find_set(e.first);
				int v = dsets.find_set(e.second);
				if ( u != v ) { 
					dsets.link(u, v); 
				}   
			}
			dsets.compress_sets(elements.begin(), elements.end());
			return dsets.count_sets(elements.begin(), elements.end());
		}

	void SmartSplit::init_col_vector(vector<int>& col_vec, int col_size)
	{
		for( int i = 0 ; i < col_size; ++i )
			col_vec.push_back(i);
	}

	// return number of partitions
	int SmartSplit::scan_and_split(string file_name, int split_factor, const int col_size, const int row_size)
	{
		rank_t rank_map;
		parent_t parent_map;
		associative_property_map<rank_t>   rank_pmap(rank_map);
		associative_property_map<parent_t> parent_pmap(parent_map);
		vector<int> col_vec;
		col_vec.reserve(col_size);
		init_col_vector(col_vec, col_size);
		queue<dep_pair> pair_queue;
		string line;
		ifstream infile;
		infile.open (file_name, std::ifstream::in);
		vector<int> dropped;
		vector<ofstream*>	outfiles;
		int real_split;
		if ( infile.good() )
		{
			while ( getline( infile, line) )
			{
				vector<string> splitted;
				trim(line);
				boost::split(splitted, line, boost::is_any_of(":"));
				// only consider col rules
				if (splitted[RULE_TYPE].compare("r") == 0 ) continue;
				ColRange col_range;
				size_t pos = splitted[LINE_RANGE].find_first_of("..");
				if (pos != string::npos )
				{
					//range
					col_range.start = stoi(splitted[LINE_RANGE].substr(0, pos));;
					col_range.end = stoi(splitted[LINE_RANGE].substr(pos + 2));

				}else if ( (pos = splitted[LINE_RANGE].find_first_of(",")) != string::npos )
				{
					//selection of cols
					vector<string> elements;
					split(elements, splitted[LINE_RANGE], boost::is_any_of(","));
					col_range.start = col_range.end = 0;
					for (auto i : elements)
						col_range.range.push_back(stoi(i));
					// dep cols
					queue_dep_cols(col_range, pair_queue);
				}else if ( stoi(splitted[LINE_RANGE]) != -1 )
				{
					// single col
					col_range.start = col_range.end = stoi(splitted[LINE_RANGE]);
				}
				// drop columns
				if (stoi(splitted[RULE_NUMBER]) == 0 )
				{
					mark_drop_columns(col_range, &dropped);
					continue;
				}

			}
			drop_columns(col_vec, dropped);
			infile.close();

			//get partitions
			int parts = partition(rank_pmap, parent_pmap, col_vec, pair_queue);
			// parts are fewer than split_factor
			real_split = parts < split_factor ? parts : split_factor;
			outfiles = open_files(real_split, file_name);
			// write out splitted cols to each file
			vector< vector<int> > part_data;
			map<int, int> rep_map;
			/*for (parent_t::iterator i = parent_map.begin(); i != parent_map.end(); ++i)
			{
				cout << i->first << "," << i->second << endl;
			}*/
			for (parent_t::iterator i = parent_map.begin(); i != parent_map.end(); ++i)
			{
				int idx;
				map<int, int>::iterator it = rep_map.find( i-> second);
				if ( it != rep_map.end() )
				{
					idx = rep_map[it->second];
					part_data.at(idx).push_back(i->first);
				}else
				{
					idx = part_data.size();
					vector<int> new_slice;
					new_slice.push_back(i->first);
					rep_map[i->second] = idx;
					part_data.push_back(new_slice);
				};
			}
			for (size_t i = 0; i < part_data.size(); ++i)
			{
				for( size_t j = 0; j < part_data.at(i).size(); ++j )
				{
					*outfiles[i % real_split] << part_data[i][j] << ",";
				}
			}
			close_files(outfiles);
		}
		return real_split;
	}

	void SmartSplit::queue_dep_cols(ColRange& col_range, queue<dep_pair> &pair_queue)
	{
		if ( col_range.start != col_range.end )
		{
			for( int i = col_range.start + 1; i < col_range.end; ++i )
			{
				dep_pair pair(i-1, i);
				pair_queue.push(pair);

			}
		}
		else
		{
			for( size_t i = 1; i < col_range.range.size(); ++i )
			{
				dep_pair pair(col_range.range.at(i-1), col_range.range.at(i));
				pair_queue.push(pair);
			}
		}

	}

	void SmartSplit::drop_columns(vector<int>& col_vec, vector<int>& dropped)
	{
		for(auto i : dropped)
		{
			col_vec.erase(col_vec.begin() + i);
		}
		col_vec.shrink_to_fit();
	}
	void SmartSplit::mark_drop_columns(ColRange& col_range, vector<int>* dropped)
	{
		// no duplicate col!!!
		if ( col_range.start != col_range.end )
		{
			for( int i = col_range.start; i < col_range.end; ++i )
				dropped->push_back(i);
		}else if ( col_range.range.size() != 0 )
		{
			for( size_t i = 0; i < col_range.range.size(); ++i )
				dropped->push_back(col_range.range.at(i));
		}else{
			dropped->push_back(col_range.start);
		}

	}

	vector<ofstream*> SmartSplit::open_files(int split, string dir)
	{
		vector<ofstream*> files;
		while(split != 0 ) 
		{
			stringstream filename;
			filename << dir;
			filename <<".";
			filename << split;
			ofstream* out = new ofstream;
			out->open(filename.str(), ofstream::out);
			files.push_back(out);
			split--;
		}
		return files;
	}

	void SmartSplit::close_files(vector<ofstream*>& out)
	{
		for(vector<ofstream*>::iterator it = out.begin(); it != out.end(); ++ it) 
		{
			(*it)->close();
		}
		// TODO: delte ofsteam, mem leak!
	}

	/*int main()
		{
		int split = SmartSplit::scan_and_split("/home/jing/tmp/test.rules", 3, 20, 30);
		cout << split << endl;
		return 0;
		}*/

}
