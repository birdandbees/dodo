#include <iostream>
#include "SmartSplit.hpp"
#include "Parser.hpp"
#include "MemWorker.hpp"
#include "IOWorker.hpp"
using namespace std;
using namespace avant_analytics;

int main()
{
	//SmartSplit
	//int split = SmartSplit::scan_and_split("/home/jing/tmp/test.rules", 3, 20, 30);
	//cout << split << endl;
	
	//Parser
	/*vector<int> cols;
	cols.push_back(0);
	cols.push_back(9);
	cols.push_back(18);
	map<int, list<OP> > col_rules;
	map<int, list<OP> > row_rules;
	list<DepRule> dep_rules;
	Parser::parse_file("/home/jing/tmp/test.rules", cols, col_rules, row_rules, dep_rules);
	cout << "for debugging" << endl;
	*/

	//MemWorker
	//MemWorker worker;
	//worker.process("/home/jing/tmp/test.rules", "/home/jing/tmp/test.rules.1", 20, 12, "/home/jing/tmp/data_file");

	//MemWorker
	IOWorker worker;
	worker.process("/home/jing/tmp/test.rules", "/home/jing/tmp/test.rules.1", 20, 12, "/home/jing/tmp/data_file");
	
	return 0;
};
