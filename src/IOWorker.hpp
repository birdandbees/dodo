#ifndef __AVANT_IOWORKER_HPP__
#define __AVANT_IOWORKER_HPP__
#include <iostream>
#include "MemWorker.hpp"
#include <vector>
#include <map>
#include <algorithm>

namespace avant_analytics
{
	class IOWorker : public MemWorker
	{
		public:
			void process(string rule_dir, string col_file, int col_size, int row_size, string data_file);
	};

}

#endif
