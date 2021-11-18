#pragma once
#include "STLpch.h"

#include "MyAlloc.hpp"
#include "Allocators/StackMemoryPool.h"
#include "Allocators/StackAllocator.hpp"


inline void CustomAllocator_Basics()
{
	fmt::print("\n---------------Custom Allocator Basics--------------------------\n");

	std::vector<int, MyAlloc<int>> vec1{ 1,2,3,4,5 };

	//std::cout << allocCount << '\n';

	printCont(vec1);

	vec1.insert(vec1.end(), { 7, 8, 9, 10, 12, 13, 4, 5, 788, 99, 80 });

	printCont(vec1);

	Vec<int> vec2{ vec1 };
	printCont(vec2);
}

inline void StackAllocator_MemoryPool()
{
	fmt::print("\n---------------StackAllocator_MemoryPool-------------------------\n");

	StackAllocator<char, false> alloc(128);

	using Str = std::basic_string<char, std::char_traits<char>, StackAllocator<char, false>>;

	Str str(alloc);

	str = "str2sttttt";
	fmt::print("max size: {0}, allocated size: {1} \n", alloc.maxSize(), alloc.allocatedSize());
	str = "wt222daaaaaaaaaaaaaaaaaaaaaaaaaaaaad2222";
	fmt::print("max size: {0}, allocated size: {1} \n", alloc.maxSize(), alloc.allocatedSize());
	


	using Str2 = std::basic_string<char, std::char_traits<char>, MyAlloc<char>>;

	Str2 str2;
	str2 = "str2sttttt";
	str2 = "wt222daaaaaaaaaaaad2222";

	
}

