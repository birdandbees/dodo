#include <map>
#include <string>
#include <vector>

typedef float (*FUNC)(vector<string>&, float);
namespace avant_analytics
{
	class MungeBits
	{

		//map<int, FUNC> func_map;
		map<int, void *> func_map;
		// amplify by factor
		inline static float multiply_by(vector<string> & args, float element)
		{
			return element * stof(args[0]);
		}

		inine void load_funcs() 
		{
			func_map[1021] = &multiply_by;
		}
	}
}
