module;   // global module fragment; optional

/* the space space between module and export module is for the headers not modularized
  and preprocessors and will not be imported or visible by other units
  */
#include <vector>


export module math;


import <algorithm>;
/* import modules can be inserted here*/


export namespace math  // anything in the namespace will be exported!
{
	int add(int first, int second);
	
	int getProduct(const std::vector<int>& vec);

	int getSum(const std::vector<int>& vec);

} // end of export namespace math
