#pragma once
#include "Templpch.h"

template<typename T, typename =std::enable_if_t<std::is_integral_v<T>, T>>
T GetInt(T&& a)
{
	std::printf("int a : %d \n", a);
	return a;
}
