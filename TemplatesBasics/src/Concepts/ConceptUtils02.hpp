#pragma once
#include "Templpch.h"

/* Before Concepts*/
template<typename T, typename U>
constexpr bool IsSameTypeOLD = std::is_same_v<T, U>;

/* With Concepts!*/
template<typename T, typename U>
concept IsSameType = std::is_same_v<T, U>;

/* With concepts*/
template<typename T, typename... Ts>
concept same_values = std::conjunction_v<std::is_same<T, Ts>...>;

template<typename T, typename... Args>
requires same_values<T, Args...>
constexpr auto addAllValues(T&& arg, Args&&...args)
{
	return (arg + ... + args);
}

