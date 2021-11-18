#pragma once
#include "Templpch.h"


template<typename T, typename... Ts>
inline constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

template<typename T, typename U>
concept same_as = std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

template<typename T, typename...Args>
concept variadic_Sumable = requires(T arg)
{
	arg + arg;															// check if type T provides + operator
	requires are_same_v<T, Args...>;
	requires sizeof...(Args) > 0;										// at least one parameter in the pack

	{arg + arg} noexcept ->same_as<T>;									// check if the sum returns a type equal to first parameter T
																		// the return type of (arg + arg) injected as the second parameter in std::same_as<T1, T2>

	requires not std::is_class_v<T> || requires {typename T::type; };	// check if T is a class and check for type defined or T is not a class 

};


template<typename T, typename...Args>
requires variadic_Sumable<T, Args...>
auto addAllValues3(T&& arg, Args&&... args)
{
	return (arg + ... + args);
}


/* Same as above but using requires rather than the concept for the constraint; prefer the above method using Concepts*/
template<typename T, typename...Args>
requires  requires(T arg)
{
	arg + arg;															// check if type T provides + operator
	requires are_same_v<T, Args...>;
	requires sizeof...(Args) > 0;										// at least one parameter in the pack

	{ arg + arg} noexcept ->same_as<T>;									// check if the sum returns a type equal to first parameter T
																		// the return type of (arg + arg) injected as the second parameter in std::same_as<T1, T2>
	requires not std::is_class_v<T> || requires {typename T::type; };	// check if T is a class then check for type defined or T is not a class; e.g. int.. 
}

auto addAllValues2(T&& arg, Args&&... args)
{
	return (arg + ... + args);
}

template<typename T>
class Rational
{
public:
	using type = T;
public:
	constexpr Rational(int numerator = 0, int denominator = 1) : num{ numerator }, denom{ denominator } {}

	constexpr Rational& operator+(const Rational& lhs) noexcept
	{
		num *= lhs.num;
		denom *= lhs.denom;

		return *this;
	}

	constexpr int Numerator() const { return num; }
	constexpr int DeNominator() const { return denom; }

private:
	int num;
	int denom;
};




