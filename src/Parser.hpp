#ifndef __AVANT_PARSER_HPP__
#define __AVANT_PARSER_HPP__

#include <boost/algorithm/string.hpp>
#include "OP.hpp"
#include "DepRule.hpp"
#include <boost/regex.hpp>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace boost;
namespace avant_analytics
{
	class Parser
	{
		public:
			static const int RULE_TYPE = 0;
			static const int LINE_RANGE = 1;
			static const int RULE_NUMBER = 2;
			static const int RULE_ARGS = 3;
			static const int RULE_ALL = -1; 
			static const int DEP_RULES  = 2000;

			static void parse(vector<string>& line, vector<int>& cols, map<int, list<OP> >& col_rules, map<int, list<OP> >& row_rules, list<DepRule>    & dep_rules);
			static void check(vector<string>& line);
			static void parse_file(string file_name, vector<int>& cols, map<int, list<OP> >& col_rules, map<int, list<OP> >& row_rules, list<DepRule>& dep_rules);
			static void generate_range(vector<int> & range, int start, int end);
	};

}

#endif
