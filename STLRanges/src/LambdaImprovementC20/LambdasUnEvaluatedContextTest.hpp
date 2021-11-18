#pragma once
#include "RangesPCH.hpp"

#include "LambdasUnEvaluatedContext.hpp"

inline void unEvaluatedContext()
{
	std::printf("\n-unEvaluatedContext-\n");

	fmt::print("add1 typeid : {}\n", typeid(add1).name());

// does not compile in MSVC
#if not defined(_MSC_VER)
	decltype(*add1) add = add2;
	add(200, 200);
#endif

	auto f = [](int a, int b) -> int
	{
		return a * b;
	};

	decltype(f) g = f;
	
}

inline void lambdasUnEvalContext()
{
	std::printf("\n-lambdasUnEvalContext-\n");

	std::set<std::string> set1 = { "didem", "Demir", "Semos", "Semsi", "salitos" };

	fmt::print("set with less<> sort :{}\n", set1);

	using SetDecreasing = std::set <std::string, decltype([](std::string_view l, std::string_view r) { return l > r; }) > ;

	SetDecreasing set2 = {"didem", "Demir", "Semos", "Semsi", "salitos"};
	fmt::print("set2 with decreasing sort :{}\n", set2);

	using SetLength = std::set < std::string, decltype([](std::string_view l, std::string_view r) {return l.size() < r.size(); }) > ;

	SetLength  set3 = { "didem", "Demir", "Semos", "Semsi", "salitos" };
	fmt::print("set3 with length sort :{}\n", set3);


}



