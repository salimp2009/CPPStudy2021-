#pragma once

#include "Templpch.h"

template<typename V, typename W>
concept IsSameHelper = std::is_same_v<V, W>;

template<typename V, typename W>
concept IsSame = IsSameHelper<V, W> && IsSameHelper<W,V>;

template<typename T>
concept AlwaysTrue = true;

template<typename T, typename U>
requires IsSame<U, T> auto add(const T& t, const U& u)
{
	std::printf("Only IsSame Concept is used!\n");
	return t + u;
}

template<typename T, typename U>
requires IsSame<T, U>&& AlwaysTrue<T> auto add(const T& t, const U& u)
{
	std::printf("AlwaysTrue Concept is used with IsSame Concept!\n");
	return t + u;
}
