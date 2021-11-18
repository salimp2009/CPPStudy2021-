#pragma once
#include "STLpch.h"
#include "AttributesCpp20.hpp"

inline void AttributesTest1()
{
	std::printf("\n-AttributesTest1-\n");

	int* val = create<int>(5);
	delete val;

	// memory leak
	create<int>(5);

	errorProneFunction();

	Mytype(5, true);

	

}
