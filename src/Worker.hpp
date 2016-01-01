#ifndef __AVANT_WORKER_HPP__
#define __AVANT_WORKER_HPP__
#include <iostream>
using namespace std;

namespace avant_analytics
{
	class Worker
	{
		public:
			virtual void process(string rule_dir, string col_file, int col_size, int row_size, string data_file) = 0;
	};
}

#endif
