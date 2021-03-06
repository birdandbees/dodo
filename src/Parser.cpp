#include "Parser.hpp"

namespace avant_analytics
{
	void Parser::parse(vector<string>& line, vector<int>& cols, map<int, list<OP> >& col_rules, map<int, list<OP> >& row_rules, list<DepRule>& dep_rules)
	{
		// trim spaces before calling this function
		// dropped columns are taken care of by smartSplit
		check(line);
		int rule_number = stoi(line[RULE_NUMBER]);
		if ( line[RULE_TYPE].compare("c") == 0 && rule_number == 0 ) return;
		sort(cols.begin(), cols.end());
		// [r|c]:<line ranges>:<rule number>:<rule args>
		string::size_type pos = line[LINE_RANGE].find_first_of("..");
		map<int, list<OP> >& rule_ptr = line[RULE_TYPE].compare("c") == 0 ? col_rules : row_rules;
		vector<int> intersection;
		if ( pos != string::npos )
		{
			// range 
			int col_start = stoi(line[LINE_RANGE].substr(0, pos));
			int col_end = stoi(line[LINE_RANGE].substr(pos + 2));
		  generate_range(intersection, col_start, col_end);	
		}else if ( (pos = line[LINE_RANGE].find_first_of(",")) != string::npos )
		{
			// selection of cols|rows
			vector<string> range_in_str;
			split(range_in_str, line[LINE_RANGE], boost::is_any_of(","));
			for(auto i : range_in_str)
			{
				intersection.push_back(stoi(i));
			}

		}else 
		{
			// single number
			if ( stoi(line[LINE_RANGE]) == -1 )
			{
				// all cols
				sort(cols.begin(), cols.end());
				copy(cols.begin(), cols.end(), intersection.begin());
			}else
			{
				intersection.push_back(stoi(line[LINE_RANGE]));
			}

		}
		// only get intersection for col-level rules
		if ( line[RULE_TYPE].compare("c") == 0 )
		{
			sort(intersection.begin(), intersection.end());
			vector<int> inter(cols.size());
			vector<int>::iterator it = set_intersection (cols.begin(), cols.end(), intersection.begin(), intersection.end(), inter.begin());
			inter.resize(it - inter.begin());
			intersection.swap(inter);
		}
		if ( intersection.size() == 0 ) return;;

		if ( rule_number > DEP_RULES )
		{
			// dependency rules
			DepRule rule;
			rule.add(intersection);
			rule.add(rule_number);
			vector<string> args;
			split(args, line[RULE_ARGS], boost::is_any_of(","));
			rule.add(args);
			dep_rules.push_back(rule);
			return;
		}
		for(auto i: intersection)
		{
			OP op;
			op.deserialize(rule_number, line[RULE_ARGS]);
			rule_ptr[i].push_back(op);
		}
	}

	//sanity check for rules
	void Parser::check(vector<string>& line)
	{
		// line size 
		assert(line.size() >= 3);
		assert(line[RULE_TYPE].compare("c") == 0 || line[RULE_TYPE].compare("r") == 0);
		const boost::regex e("\\d+");
		assert(regex_match(line[RULE_NUMBER], e));
	}

	void Parser::parse_file(string file_name, vector<int>& cols, map<int, list<OP> >& col_rules, map<int, list<OP> >& row_rules, list<DepRule>& dep_rules)
	{
		string line;
		ifstream infile;
		infile.open (file_name, std::ifstream::in);
		if ( infile.good() )
		{
			while ( getline( infile, line) )
			{   
				vector<string> splitted;
				trim(line);
				boost::split(splitted, line, boost::is_any_of(":"));
				parse(splitted, cols, col_rules, row_rules, dep_rules);
			}   
		}
		infile.close();
	}

	void Parser::generate_range(vector<int> & range, int start, int end)
	{
		for (int i = start; i <= end; ++i)
		{
			range.push_back(i);
		}
	}

}
