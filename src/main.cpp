#include <iostream>
#include "SmartSplit.hpp"
#include "Parser.hpp"
#include "IOWorker.hpp"
#include "MemWorker.hpp"
#include <iostream>
#include <string>
using namespace std;
using namespace avant_analytics;

int main(int argc, char* argv[])
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

	//IOWorker
	//IOWorker worker;
	//worker.process("/home/jing/tmp/test.rules", "/home/jing/tmp/test.rules.1", 20, 12, "/home/jing/tmp/data_file");

	string rule_file;
	string col_file;
	string split = "0";
	string data_file;
	string col;
	string row; 
	bool io = 0;

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {
			return 0;
		} else if ((arg == "-s") || (arg == "--split")) {
			if (i + 1 < argc) { 
				i++;
				split = argv[i++];
				rule_file = argv[i++]; 
				col = argv[i++];
				row = argv[i++];
				cout << "split " << split << "," << rule_file << "," << col << "," << row << endl;

			} else { 
				std::cerr << "--destination option requires one argument." << std::endl;
				return 1;
			}   
		} else if ((arg == "-p") || (arg == "--proc")) {
			if (i + 1 < argc) { 
				i++;
				rule_file = argv[i++];
				col_file = argv[i++];
				col = argv[i++];
				row = argv[i++];
				data_file = argv[i++];
				cout << "proc " << rule_file << "," << col << "," << row << "," << data_file << endl;
			} else { 
				std::cerr << "--destination option requires one argument." << std::endl;
				return 1;
			}
	} else if ((arg == "-i") || (arg == "--io")) {
		if (i + 1 < argc) { 
			i++;
			rule_file = argv[i++];
			col_file = argv[i++];
			col = argv[i++];
			row = argv[i++];
			data_file = argv[i++];
			io = 1;
			cout << "proc " << rule_file << "," << col << "," << row << "," << data_file << endl;
		} else { 
			std::cerr << "--destination option requires one argument." << std::endl;
			return 1;
		}
	}
}
if ( stoi(split) )
{
	SmartSplit::scan_and_split(rule_file, stoi(split), stoi(col), stoi(row));

}else if ( io )
{
	IOWorker worker;
	worker.process(rule_file, col_file, stoi(col), stoi(row), data_file);
}else
{
	MemWorker worker;
	worker.process(rule_file, col_file, stoi(col), stoi(row), data_file);
}

return 0;
};
