#include "Parser.hpp"

namespace avant_analytics
{
	void Parser::parse(vector<string>& line, vector<int>& cols, map<int, list<OP> >& col_rules)
	{
		// TODO: need refactoring
		if (line.size() == 2 )
		{
			cols[stoi(line[1])] = 0;
			return;
		};
		string::size_type pos = line[LINE_RANGE].find_first_of("..");
		string col_end = line[LINE_RANGE];
		string col_start = line[LINE_RANGE];
		// only col rules for now, skip first element
		// 	// parse line range
		// 		// [r|c]:<line ranges>:<rule number>:<rule args>
		if ( pos != string::npos )
		{
			col_start = line[LINE_RANGE].substr(0, pos);
			col_end = line[LINE_RANGE].substr(pos + 2);
		}
		string rule_number = line[RULE_NUMBER];
		int col_flag = stoi(rule_number) == 0 ? 1 : 2;
		// it's a sub-proc rule
		if (line.size() > 4)
		{
			cols[stoi(col_start)] = col_flag;
			for( int i = 2; i < line.size(); i += 2 )
			{
				if ( line[i].size() != 0 )
				{
					OP op(stoi(line[i]), deserialize_rule_args(line[i+1]));
					col_rules[stoi(col_start)].push_back(op); 
				}
			}
			return;

		}
		// drop columns and set cols vector
		for ( int i = stoi(col_start); i <= stoi(col_end); ++i)
		{
			cols[i] = col_flag;
			OP op(stoi(rule_number), deserialize_rule_args(line[RULE_ARGS]));
			col_rules[i].push_back(op);
		}
	}
}
