#pragma once
#include "ConstraintAddAll.hpp"

template<typename T, typename... Args>
concept Addable = requires(T arg)
{
	(arg + arg);
};

template<typename... Args>
concept PackHasElements = sizeof...(Args) > 0;

template<typename T>
concept AddReturnsSameType = requires(T arg)
{
	{arg + arg} noexcept ->same_as<T>;
};

template<typename T>
concept ClassWithTypeorNotAClass = not std::is_class_v<T> || requires {typename T::type; };
	

template<typename T, typename... Args>
requires Addable<T, Args...>&& are_same_v<T, Args...> &&
		PackHasElements<Args...>&& AddReturnsSameType<T> &&
		ClassWithTypeorNotAClass<T>
auto AddAllValues4(T&& arg, Args&&... args)
{
	return (arg + ... + args);
}


/* ALTERNATIVE WAY; using the Concepts in  Requires clause to constrain the function*/
template<typename T, typename... Args>
requires requires
{
	requires Addable<T, Args...>;
	requires are_same_v<T, Args...>;
	requires PackHasElements<Args...>;
	requires AddReturnsSameType<T>;
	requires ClassWithTypeorNotAClass<T>;
}

auto AddAllValues5(T&& arg, Args&&... args)
{
	return (arg + ... + args);
}