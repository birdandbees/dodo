#ifndef __AVANT_MUNGE_BITS_HPP__
#define __AVANT_MUNGE_BITS_HPP__
#include <map>
#include <string>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

typedef float (*FUNC)(vector<string>&, float);
typedef Matrix<float, Dynamic, 1> COLUMN;
typedef bool (*MFUNC) (vector<string>&,  vector< Ref<COLUMN> >& cols);
namespace avant_analytics
{
	class MungeBits
	{
		public:
			// amplify by factor
			inline static float multiply_by(vector<string> & args, float element)
			{
				return element * stof(args[0]);
			}

			inline static float filter_by_function(vector<string> & args, float element)
			{
				// do nothing;
				return 0;
			}

			inline static void load_funcs(map<int, FUNC>& func_map) 
			{
				func_map[1021] = &multiply_by;
				func_map[3021] = &filter_by_function;;
			}

			inline static bool n_times_mean(vector<string>& args, vector< Ref< Matrix<float, Dynamic, 1> > >&cols)
			{

				cols.at(0) = cols.at(0) * cols.at(0).mean() * stof(args[0]);
				return true;
			}
			inline static void load_funcs(map<int, MFUNC>& func_map)
			{
				func_map[1501] = &n_times_mean;
			}
	};
}
#endif
