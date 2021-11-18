#pragma once

#include "Templpch.h"
#include "EmptyBaseClassOptm/EmptyBaseClassOptimization.hpp"

inline void EmptyBaseClassTest1()
{
	fmt::print("size of EmptyClass: {}\n", sizeof (EmptyClass));
	fmt::print("size of Empty: {0}, EmptyToo: {1}, EmptyThree: {2}\n", sizeof(Empty), sizeof(EmptyToo), sizeof(EmptyThree));
	fmt::print("size of Empty: {0}, EmptyToo: {1}, EmptyThree: {2}\n", sizeof(Empty), sizeof(EmptyToo), sizeof(EmptyThree));


}
