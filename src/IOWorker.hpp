#include <iostream>

namespace avant_analytics
{
	class IOWorker : public Worker
	{

		public:
			map<int, list<OP> > read(string rule_file);
			void process();


	}

}
