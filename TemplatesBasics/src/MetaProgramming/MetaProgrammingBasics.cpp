#include "Templpch.h"
#include "MetaProgrammingBasics.hpp"

void metaprog_TypeManipulation()
{
	std::printf("\n--metaprog_TypeManipulation--\n");
	auto arg = std::vector{ 1,2,3,4,5 };
	static_cast<std::remove_reference<decltype(arg)>::type&&>(arg);

	// using std::same_as Concept
	static_assert(std::same_as<std::vector<int>, decltype(arg)>);
	
	// using std::is_same_v Type_trait
	static_assert(!std::is_same_v<std::vector<int>, decltype(static_cast<std::remove_reference<decltype(arg)>::type&&>(arg))>);

	static_assert(std::is_same_v<int, removeConst<int>::type>);
	static_assert(std::is_same_v<int, removeConst<const int>::type>);

	static_assert(std::is_same_v<int, removeReference<int>::type>);
	static_assert(std::is_same_v<int, removeReference<int&>::type>);
	static_assert(std::is_same_v<int, removeReference<int&&>::type>);
	
	// Pointers are not treated as references ???
	static_assert(!std::is_same_v<int, removeReference<int*>::type>);
	static_assert(!std::is_same_v<int, std::remove_reference_t<int*>>);

}

void metaprog_MetaFunctvsRunTime()
{
	auto result1=PowerRunTime(2, 10);
	fmt::print("Run time result: {}\n", result1);

	static_assert(Power<2, 10>::value == 1024);
	static_assert(PowerHybrid(2, 10) == 1024);
}

