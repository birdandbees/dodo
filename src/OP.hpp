#ifndef __AVANT_OP_HPP__
#define __AVANT_OP_HPP__
#include <string>
#include <vector>
#include <string>
using namespace std;
namespace avant_analytics
{
	struct OP
	{
		int op_id;
		vector<string> params;
		public:
		OP(){};
		OP(int op, vector<string> args); 
		void deserialize(const string& config); 
		void deserialize(int rule_number, const string& config); 
		string serialize();
	};
}

#endif
