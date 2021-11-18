
#include <cstdio>
#include <vector>
#include <format>

#include <spdlog/fmt/fmt.h>

import mathTemp;
import math;
import partmath;
import mathPart;



int main()
{
	std::printf("test mathh module; add(): %d\n", math::add(10, 20));
	//fmt::print("test mathh module; add(): {0}\n", add(10, 20));

	auto var =math::add(15, 20);
	fmt::print("test math module; add(): {0}\n", var);

	std::vector<int> vec1{ 1,2,3 };

	
	fmt::print("getproduct: {}\n", math::getProduct(vec1));
	std::printf("test math module; getproduct(): %d\n", math::getProduct(vec1));
	std::printf("test math module; getsum(): %d\n", math::getSum(vec1));

	fmt::print("add2 from sub modules: {}\n", add2(5, 6));
	fmt::print("mult from sub modules: {}\n", mult(5,6));

	fmt::print("add3 from partitioned module: {}\n", add3(7, 8));
	fmt::print("mult2 from partitioned module: {}\n", mult2(7, 8));
	
	fmt::print("sum from template module: {}\n", tmath::sum(7, 8));

	auto [value, type] = tmath::sum2(10, 5);
	fmt::print("sum2 from template module: value:{0}, type:{1}\n", value, type);


}