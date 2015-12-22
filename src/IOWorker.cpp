#include "IOWorker.hpp"
#include "Parser.hpp"

namespace avant-analytics
{

	map<int, list<OP> > IOWorker::read(string rule_dir)
	{
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

	void IOWorker::process(string rule_dir, int col_size, int row_size, string data_file, map<int, FUNC>& func_map)
	{
		ofstream out;
		out.open(rule_dir + ".out", ofstream::out);
		map<int, list<OP> > rules = read(rule_dir);
		ifstream data;
		data.open(data_file, ifstream::in);
		if ( data.good() )
		{
			while(getline(data, line))
			{
				vector<string> row;
				split(row, line, boost::is_any_of(","));
				//apply FUNC
				for (int i = 0;  i< row.size(); ++i)
				{

					switch (cols[i])
					{
						case 0:
							out << row[i];
							out << ",";
							break;
						case 2:
							list<OP> list_of_rules = (list<OP>) rules[i];
							float value =  stof(row[i]);
							for (list<OP>::iterator it = list_of_rules.begin(); it != list_of_rules.end(); ++it )
							{
								value = func_map[(*it).op_id]((*it).params, value);
							}	
							out << value;
							out << ",";
					}

				}
				out << endl;
			}
		}
		data.close();
		out.close();

	}

}
