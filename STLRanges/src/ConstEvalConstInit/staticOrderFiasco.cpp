#include "RangesPCH.hpp"

inline int squareStatic(int n)
{
	return n * n;
}

// Lazy intiliazation from Thread Safe Richard Meyers singleton to avoid Static Initialization order problem before c++20
int& staticA()
{
	static auto staticValA = squareStatic(5);

	return staticValA;
}


// C++20 way to avoid Static Initialization order problem

constexpr int squareStatic2(int n)
{
	return n * n;
}

constinit auto staticValA2 = squareStatic2(5);
