#ifndef __AVANT_DEPRULE_HPP__
#define __AVANT_DEPRULE_HPP__
#include <string>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
using namespace std;
namespace avant_analytics
{
	struct DepRule 
	{
		int op_id;
		vector<string> params;
		vector<int> range;
		inline void add(vector<string>& args){ copy(args.begin(), args.end(), back_inserter(params));};
		inline void add(int rule_number) { op_id = rule_number; };
		inline void add(vector<int>& range_vec) { copy(range_vec.begin(), range_vec.end(), back_inserter(range)); };
	};
}
#endif
