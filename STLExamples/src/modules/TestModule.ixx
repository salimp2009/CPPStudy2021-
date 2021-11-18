#include "STLpch.h"

export module TestModule;

int mymoduleVariable = 500;

export int MyModuleFunc(int n)
{
	std::printf("\n----modules test----\n");
	return n + mymoduleVariable;
}
