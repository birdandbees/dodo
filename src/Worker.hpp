#include <iostream>
using namespace std;
using namespace boost;

namespace avant_analytics
{

	class Worker
	{
		map<int, list<OP> >& rules;

		public:
		virtual map<int, list<OP> > read(string rule_file) = 0;
		virtual void process() = 0;
	}


}
