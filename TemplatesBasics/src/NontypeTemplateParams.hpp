#pragma once
#include "Templpch.h"

template<auto T>
class Message
{
public:
	void printMsg()
	{
		fmt::print("type: {}\n", T);
	}
};
