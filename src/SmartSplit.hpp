#ifndef __AVANT_SMART_SPLIT_HPP__
#define __AVANT_SMART_SPLIT_HPP__
#include <iostream>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <queue>
#include <map>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <fstream>
#include <new>
#include <cstddef>
#include <string>

// heart of split class
using namespace std;
using namespace boost;

namespace avant_analytics
{
	struct ColRange
	{
		int start;
		int end;
		vector<int> range;
	};

	typedef pair<int, int> dep_pair;
	typedef map<int,std::size_t> rank_t; // => order on int
	typedef map<int,int> parent_t;

	class SmartSplit
	{
		public:
			static const int RULE_TYPE = 0;
			static const int LINE_RANGE = 1;
			static const int RULE_NUMBER = 2;
			static const int RULE_ARGS = 3;
			static const int RULE_ALL = -1;
			static const int DEP_RULES  = 2000;

			static int scan_and_split(string file_name, int split_factor, const int col_size, const int row_size);
			template <typename Rank, typename Parent>
				static int partition(Rank& r, Parent& p, vector<int>& elements, queue<dep_pair>& pair_queue);
			typedef pair<int, int> dep_pair;
			typedef map<int,std::size_t> rank_t; // => order on int
			typedef map<int,int> parent_t;

		private:
			static void init_col_vector(vector<int>& col_vec, int col_size);
			static void queue_dep_cols(ColRange& col_range, queue<dep_pair> &pair_queue);
			static void drop_columns(vector<int> & col_vec, vector<int>& dropped);
			static void mark_drop_columns(ColRange& col_range, vector<int>* dropped);
			static vector<ofstream*> open_files(int split, string dir);
			static void close_files(vector<ofstream*>& out);
	};
}

#endif
