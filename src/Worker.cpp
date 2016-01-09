#include "Worker.hpp"

namespace avant_analytics
{

	void Worker::read(string rule_dir,vector<int>& cols, map<int, list<OP> >& col_rules, map<int, list<OP> >& row_rules, list<DepRule>& dep_rules )
	{
		Parser::parse_file(rule_dir, cols, col_rules, row_rules, dep_rules);
	}
	void Worker::load_cols(string col_file, vector<int>& cols)
	{
		ifstream col_file_input;
		col_file_input.open(col_file, std::ifstream::in);
		string line;
		while ( getline(col_file_input, line) )
		{   
			vector<string> splitted;
			boost::split(splitted, line, boost::is_any_of(","));
			for (auto s : splitted)
			{   
				if (!s.empty())
					cols.push_back(stoi(s));
			}   

		}   
		col_file_input.close();
		sort(cols.begin(), cols.end());
	}
}
