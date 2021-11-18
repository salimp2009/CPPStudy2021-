#pragma once
#include "FunctionRefImpt.hpp"
#include "Experimental/IntrusiveSharedPtrImpl.hpp"

int foo(function_ref<int()>f)
{
	return f();
}


inline void functionref_Tests()
{
	std::printf("\n----functionref_Tests()---------\n");
	auto functLamb = [i = 0]()mutable noexcept { return ++i; };
	std::cout << foo(functLamb) << '\n';

	const function_ref<int()> getNumber= functLamb;
	std::cout << getNumber() << '\n';

	const std::function<int()> getNumberSTDFunc = functLamb;
	std::cout << sizeof getNumber << '\n';
	std::cout << sizeof getNumberSTDFunc << '\n';
}
