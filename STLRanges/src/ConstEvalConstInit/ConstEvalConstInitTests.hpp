#pragma once
#include "RangesPCH.hpp"
#include "ConstEvalConsInit.hpp"

constexpr auto res1 = square(5);

// constinit does not imply constness
constinit auto res2 = square(5);

inline void constEvalSqr()
{
	std::printf("\n-constEvalSqr-\n");
	constexpr auto result = res1;
	constinit static int startvalue = 20;
	
	// constinit has to be static or thread_storage 
	//constinit auto result2 = res2;
	static_assert(result == 5 * 5);

	const int a = 5;
	auto result3 = res2;
	fmt::print("square(a): {0}, square(5): {1}", square(a), square(5));
	static_assert(result == square(a));
	static_assert(result == square(5));

	constinit thread_local auto threadData = square(5);
	// does not Compile it ask for constness
	//static_assert(threadData == square(5));

	int result4 = square(5);
}

inline void constEvalInitExprSqr()
{
	std::printf("\n- constEvalInitExprSqr-\n");

	// Does not compile needs to be consteval or constexpr
	//constexpr auto value1 = sqrRunTime(5);

	// initialized with consteval
	constexpr auto value2 = sqrCompileTime(5);
	constinit static auto value2a = sqrCompileTime(5);
	auto value3 = sqrCompileTime(5);
	
	// initialized with constexpr; compile or runtime
	constinit static auto value2b = sqrCompileOrRunTime(5);
	constexpr auto value4 = sqrCompileOrRunTime(5);

	int x = 100;
	const int x2 = 110;
	constexpr int x3 = 120;

	int prod1 = sqrRunTime(x);
	int prod2 = sqrCompileOrRunTime(x);
	
	// Wont compile since is 
	//int prod3 = sqrCompileTime(x);

	// ok; since x2 is const or it can be constexpr (better) as x3
	int prod4 = sqrCompileTime(x2);
	int prod5 = sqrCompileTime(x3);
}


extern int staticValB;
extern int staticValB2;

inline void staticInitializationFiasco()
{
	std::printf("\n-staticInitializationFiasco-\n");

	std::printf("initialized with a static local variable inside a function: %d\n", staticValB);
	std::printf("initialized with C++20 constinit: %d\n", staticValB2);
}