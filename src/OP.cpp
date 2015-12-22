#include "OP.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <iostream>
#include <boost/algorithm/string.hpp>
//using namespace std;
using namespace boost;

namespace avant_analytics
{

	OP::OP(int op, vector<string> args): op_id(op), params(args)
	{
		//op_id = op;
		//params = args;
	}

	void OP::deserialize(const string& config)
	{
		vector<string> tokens;
		split(tokens, config, boost::is_any_of(":"));
		op_id = stoi(tokens[0]);
		vector<string> args;
		split(args, tokens[1], boost::is_any_of(","));
		params = args;
	}


	string OP::serialize()
	{
		stringstream result;
		result << op_id;
		result << ":";
		for( vector<string>::iterator it = params.begin(); it != params.end(); ++ it) 
		{
			result << (*it) << ",";
		}
		return result.str();

	}


	/*int main()
		{
		vector<string> ss;
	//ss.push_back("test");
	OP op(12, ss);
	cout << op.serialize() << endl;

	map<int, list<OP> > op_list;
	list<OP> element;
	element.push_back(op);
	op_list[12] = element;

	string test_string = "1021:5,6";
	OP op;
	op.deserialize(test_string);
	cout << op.serialize() << endl;

	}*/
}
