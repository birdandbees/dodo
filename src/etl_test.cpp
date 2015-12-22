#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <cstddef>
#include <boost/algorithm/string.hpp>
#include "OP.hpp"
#include <new>

#define LINE_RANGE 1
#define RULE_NUMBER 2
#define RULE_ARGS 3

using namespace std;
using namespace boost;
using namespace avant_analytics;

typedef float (*FUNC)(vector<string>&, float);

// forward declaration
void parse(vector<string>&, vector<int>&, map<int, list<OP> >&);
string serialize_rule_args(vector<string>& );
vector<string> deserialize_rule_args(string);
void init_cols(vector<int>&, int);

void parse_rules(string filename, int col_size, int row_size, vector<int>& cols, map<int, list<OP> >& col_rules)
{

	string line;
	ifstream infile;
	infile.open (filename, std::ifstream::in);
	if ( infile.good() )
	{
		while ( getline( infile, line) )
		{
			// split by :
			// TODO: strip spaces
			vector<string> splitted;
			boost::split(splitted, line, boost::is_any_of(":"));
			parse(splitted, cols, col_rules);

		}
	}
	infile.close();
}

float multiply_by(vector<string> & args, float element)
{
	return element * stof(args[0]);
}

// load rule number and function pointers into a map
void load_funcs(map<int, FUNC>& func_map) 
{
	func_map[1021] = &multiply_by;
}


void parse(vector<string>& line, vector<int>& cols, map<int, list<OP> >& col_rules)
{
	// TODO: need refactoring
	if (line.size() == 2 )
	{
		cols[stoi(line[1])] = 0;
		return;


	};

	// only col rules for now, skip first element
	// parse line range
	// [r|c]:<line ranges>:<rule number>:<rule args>
	string::size_type pos = line[LINE_RANGE].find_first_of("..");
	string col_end = line[LINE_RANGE];
	string col_start = line[LINE_RANGE];
	if ( pos != string::npos )
	{
		col_start = line[LINE_RANGE].substr(0, pos);
		col_end = line[LINE_RANGE].substr(pos + 2);
	}
	string rule_number = line[RULE_NUMBER];
	int col_flag = stoi(rule_number) == 0 ? 1 : 2;
	// it's a sub-proc rule
	if (line.size() > 4)
	{
		cols[stoi(col_start)] = col_flag;
		for( int i = 2; i < line.size(); i += 2 )
		{
			if ( line[i].size() != 0 )
			{
				OP op(stoi(line[i]), deserialize_rule_args(line[i+1]));
				col_rules[stoi(col_start)].push_back(op); 
			}
		}
		return;

	}
	// drop columns and set cols vector
	for ( int i = stoi(col_start); i <= stoi(col_end); ++i)
	{
		cols[i] = col_flag;
		OP op(stoi(rule_number), deserialize_rule_args(line[RULE_ARGS]));
		col_rules[i].push_back(op);
	}
}

vector<ofstream*> open_files(int split, string dir)
{
	vector<ofstream*> files;
	while(split != 0 )
	{
		stringstream filename;
		filename << dir;
		filename <<".";
		filename << split;
		ofstream* out = new ofstream;
		out->open(filename.str(), std::ofstream::out);
		files.push_back(out);
		split--;
	}
	return files;
}

void close_files(vector<ofstream*>& out)
{
	for(vector<ofstream*>::iterator it = out.begin(); it != out.end(); ++ it)
	{
		//out[i]->close();
		(*it)->close();
	}
	// TODO: delte ofsteam, mem leak!
}

string serialize_rule_args(vector<string>& args)
{
	stringstream result;
	for( vector<string>::iterator it = args.begin(); it != args.end(); ++ it)
	{
		result << (*it) << ",";
	}
	return result.str();
}
vector<string> deserialize_rule_args(string args)
{
	vector<string> result;
	split(result, args, boost::is_any_of(","));
	return result;
}

// split and dump to rule files
void data_split(int split_factor, string dir, vector<int>& cols, map<int, list<OP> >& col_rules )
{
	// split
	//dump rules out
	vector<ofstream*> outfiles = open_files(split_factor, dir);
	int idx = 0;
	//for ( vector<int>::iterator it = cols.begin(); it != cols.end(); ++it )
	for ( int col = 0; col < cols.size(); ++col )
	{
		// get index
		switch (cols[col])
		{
			case 0:
				*outfiles[idx % split_factor] << "c:";
				*outfiles[idx % split_factor] << col;
				*outfiles[idx % split_factor] << endl;
				break;
			case 1:
				break;
			case 2: 
				*outfiles[idx % split_factor] << "c:";
				*outfiles[idx % split_factor] << col ;
				list<OP> list_of_rules = col_rules[col];
				for (list<OP>::iterator it = list_of_rules.begin(); it != list_of_rules.end(); ++it)
				{
					*outfiles[idx % split_factor] << ":";
					*outfiles[idx % split_factor] << (*it).serialize();
					//*outfiles[idx % split_factor] << ":" << (*it).op_id << ":";
					//*outfiles[idx % split_factor] << serialize_rule_args( (*it).params );
					//*outfiles[idx % split_factor] << ":";
				}	
				*outfiles[idx % split_factor] << endl;
				break;
		};
		idx++;

	}

	close_files(outfiles);
}

void process(string rule_dir, int col_size, int row_size, string data_file, map<int, FUNC>& func_map)
{
	//open output
	ofstream out;
	out.open(rule_dir + ".out", ofstream::out);
	ifstream rule_file;
	rule_file.open(rule_dir, ifstream::in);
	map<int, list<OP> > rules;
	vector<int> cols;
	cols.resize(col_size);
	init_cols(cols, 1);
	string line;
	while ( getline(rule_file, line) )
	{	
		vector<string> splitted;
		boost::split(splitted, line, boost::is_any_of(":"));
		parse(splitted, cols, rules);
	}
	rule_file.close();
	ifstream data;
	data.open(data_file, ifstream::in);
	if ( data.good() )
	{
		while(getline(data, line))
		{
			vector<string> row;
			split(row, line, boost::is_any_of(","));
			// apply FUNC
			for (int i = 0;  i< row.size(); ++i)
			{

				switch (cols[i])
				{
					case 0:
						out << row[i];
						out << ",";
						break;
					case 2:
						list<OP> list_of_rules = (list<OP>) rules[i];
						float value =  stof(row[i]);
						for (list<OP>::iterator it = list_of_rules.begin(); it != list_of_rules.end(); ++it )
						{
							value = func_map[(*it).op_id]((*it).params, value);
						}	
						out << value;
						out << ",";
				}

			}
			out << endl;
		}
	}
	data.close();
	out.close();
}
void init_cols(vector<int>& cols, int value)
{
	for (vector<int>::iterator it = cols.begin(); it != cols.end(); ++it )
	{
		*it = value;
	}
};


int main(int argc, char* argv[])
{
	string rule_file;
	string split = "0";
	string data_file;
	string col;
	string row;	
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {
			//show_usage(argv[0]);
			return 0;
		} else if ((arg == "-s") || (arg == "--split")) {
			if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				i++;
				split = argv[i++];
				rule_file = argv[i++]; // Increment 'i' so we don't get the argument as the next argv[i].
				col = argv[i++];
				row = argv[i++];
				cout << "split " << split << "," << rule_file << "," << col << "," << row << endl;

			} else { // Uh-oh, there was no argument to the destination option.
				std::cerr << "--destination option requires one argument." << std::endl;
				return 1;
			}  
		} else if ((arg == "-p") || (arg == "--proc")) {
			if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				i++;
				rule_file = argv[i++];
				col = argv[i++];
				row = argv[i++];
				data_file = argv[i++];
				cout << "proc " << rule_file << "," << col << "," << row << "," << data_file << endl;
			} else { // Uh-oh, there was no argument to the destination option.
				std::cerr << "--destination option requires one argument." << std::endl;
				return 1;
			}  
		} 
	}

	if ( stoi(split) )
	{
		map<int, list<OP> > col_rules;
		vector<int> cols;
		cols.resize(stoi(col));
		init_cols(cols, 0);
		parse_rules(rule_file, stoi(col), stoi(row), cols, col_rules);
		data_split(stoi(split), rule_file, cols, col_rules);

	}else
	{
		map<int, FUNC> func_map;
		load_funcs(func_map);
		process(rule_file, stoi(col), stoi(row), data_file, func_map);
	}

}




