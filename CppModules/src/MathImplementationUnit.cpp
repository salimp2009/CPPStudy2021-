module;

#include <numeric>


module math;
/* the example shows numeric to be here but then it does compile with a warning because this is the field export / import 
	and standart library is not modularized ; might be a false warning !!!!
*/


//import <algorithm>;

namespace math
{
	 int add(int first, int second)
	{

		
		 return first + second;

	}

	int getProduct(const std::vector<int>& vec)
	{
		return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<int>());
	}

	int getSum(const std::vector<int>& vec)
	{
		int sum = 0;
		std::for_each(vec.begin(), vec.end(), [&sum](auto& elem) {sum += elem* elem; });
		return sum;
	}


}



