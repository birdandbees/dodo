#ifndef __AVANT_WORKER_HPP__
#define __AVANT_WORKER_HPP__
#include <iostream>
#include "Parser.hpp"
#include <vector>

using namespace std;

namespace avant_analytics
{
	class Worker
	{
		public:
			virtual void process(string rule_dir, string col_file, int col_size, int row_size, string data_file) = 0;
		  void read(string rule_dir,vector<int>& cols, map<int, list<OP> >& col_rules, map<int, list<OP> >& row_rules, list<DepRule>& dep_rules);
			void load_cols(string col_file, vector<int>& cols);
	};
}

#endif
