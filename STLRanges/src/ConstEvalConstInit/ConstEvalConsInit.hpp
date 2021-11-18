#pragma once

//The consteval specifier declares a function or function template to be an immediate function; compile time 
consteval int square(int value)
{
	return value * value;
}

inline int sqrRunTime(int n)
{
	return n * n;
}

consteval int sqrCompileTime(int n)
{
	return n * n;
}

constexpr int sqrCompileOrRunTime(int n)
{
	return n * n;
}



