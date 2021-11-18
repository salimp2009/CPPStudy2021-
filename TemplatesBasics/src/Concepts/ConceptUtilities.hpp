#pragma once
#include "Templpch.h"

template<typename T>
requires std::integral<T>
constexpr std::integral auto greaterCommonDivisor(T a, T b)
{
	if (b == 0) return a;
	
	return greaterCommonDivisor(b, a % b);
}

template<typename T>
constexpr auto greaterCommonDivisor1(T a, T b) requires std::integral<T>
{
	if (b == 0) return a;

	return greaterCommonDivisor1(b, a % b);
}

template<std::integral T>
constexpr auto greaterCommonDivisor2(T a, T b) 
{
	if (b == 0) return a;

	return greaterCommonDivisor2(b, a % b);
}


constexpr std::integral auto greaterCommonDivisor3(std::integral auto a, std::integral auto b)
{
	if (b == 0) return a;

	return greaterCommonDivisor3(b, a % b);
}

constexpr  auto greaterCommonDivisor4( auto a,  auto b)
{
	if (b == 0) return a;

	return greaterCommonDivisor4(b, a % b);
}


template<unsigned i>
requires (i <= 20) // not a preferred method to use prefer more name concepts here 
int sum(int j)
{
	return j + i;
}


template<std::integral... Args>
constexpr bool allValid(Args... args) { return (... && args); };

template<std::integral... Args>
constexpr bool anyValid(Args... args) { return (... || args); };

template<std::integral... Args>
constexpr bool noneValid(Args... args) { return not(... || args); };

template<std::forward_iterator It>
constexpr void advanceIt(It& iter, int n)
{
	std::printf("forward iterator\n");
}

template<std::bidirectional_iterator It>
constexpr void advanceIt(It& iter, int n)
{
	std::printf("bidirectional_iterator\n");
}

template<std::random_access_iterator It>
constexpr void advanceIt(It& iter, int n)
{
	std::printf("random_access_iterator\n");
}

template<typename T>
struct DVector
{
	DVector() { std::printf("DVector<T>\n"); }
};

template<std::regular Reg>
struct DVector<Reg>
{
	DVector() { std::printf("DVector<std::regular>\n"); }
};

void overload(auto t)
{
	fmt::print("auto t: {}\n", t);
}
void overload(std::integral auto t)
{
	fmt::print("std::integral auto t: {}\n", t);
}

void overload(long t)
{
	fmt::print("long t: {}\n", t);
}


