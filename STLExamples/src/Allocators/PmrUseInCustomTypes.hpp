#pragma once

#include "STLpch.h"

class PmrCustomer
{
private:
	/* pmr::string variable will also store the pass allocator*/
	std::pmr::string name; 
public:
	//using allocator_type = std::pmr::polymorphic_allocator<char>;
	/*Alternative to be more flexible*/
	using allocator_type = decltype(name)::allocator_type;

	PmrCustomer(std::pmr::string n, allocator_type alloc = {}) :name{ std::move(n), alloc } 
	{
	}

	PmrCustomer(const PmrCustomer& c, allocator_type alloc) :name{c.name, alloc }
	{
		std::printf("copy constructor\n");
	}

	PmrCustomer(PmrCustomer&& c, allocator_type alloc) :name{std::move(c.name), alloc }
	{
		std::printf("move constructor\n");

	}

	void setName(std::pmr::string s) { name = std::move(s); }

	std::pmr::string getName() const { return name; }

	std::string getNameAsString() const { return std::string{ name }; }

};
