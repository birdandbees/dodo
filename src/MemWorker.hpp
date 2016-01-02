#ifndef __AVANT_MEM_WORKER_HPP__
#define __AVANT_MEM_WORKER_HPP__
#include "MemWorker.hpp"
#include "Parser.hpp"
#include "MungeBits.hpp"
#include "Worker.hpp"
#include "DepRule.hpp"
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
#include <algorithm>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <fstream>

using namespace boost;
using namespace Eigen;
using namespace std;

namespace avant_analytics
{
	class MemWorker : public Worker
	{
		public:
			const char DELIMITER = ',';
			const char NEW_LINE = 10;
			void process(string rule_dir, string col_file, int col_size, int row_size, string data_file);
			void read(string rule_dir,vector<int>& cols, map<int, list<OP> >& col_rules, map<int, list<OP> >& row_rules, list<DepRule>& dep_rules);
			void load_cols(string col_file, vector<int>& cols);
			void check (int test, const char * message, ...);
			char*  load_data(string data_file, size_t& size);

	};
}

#endif
