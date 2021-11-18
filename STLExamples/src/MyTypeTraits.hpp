#pragma once
#include "STLpch.h"

//#include <iostream>
//#include <type_traits>

template<typename T>
void TypeFoo(T&& val)
{
	if (std::is_pointer_v<std::decay_t<decltype(val)>>)
	{
		std::cout << "foo() called with a pointer \n";
	}
	else
	{
		std::cout << "foo() called with a value \n";
	}
}

template <typename T>
constexpr void TypeFoo2(const T& val)
{
	if constexpr (std::is_pointer<T>::value)
	{
		std::cout << *val << '\n';
	}
	
	else
	{
		std::cout << val << '\n';
	}
}

template<typename T>
void TypeFoo3_Impl(T&& val, std::true_type)
{
	std::cout<< "Pointer type called: " << *val << '\n';
}

template<typename T>
void TypeFoo3_Impl(T&& val, std::false_type)
{
	std::cout << "Value called: " << val << '\n';
}


template<typename T>
void TypeFoo3(T&& val)
{
	TypeFoo3_Impl<T>(std::forward<T>(val), std::is_pointer<std::decay_t<decltype(val)>>());
}

