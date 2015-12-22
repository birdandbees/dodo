#include <iostream>

namespace avant_analytics
{
	class MemWorker : public Worker
	{

		public:
			map<int, list<OP> > read(string rule_file);
			void process();


	}

}
