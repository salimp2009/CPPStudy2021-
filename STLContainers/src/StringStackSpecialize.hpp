#pragma once

#include "STLContpch.h"

#include "Stack2VectorCont.hpp"

/*Full specialization of SPStack2*/
template<>
class SPStack2<std::string>
{
private:
	std::deque<std::string> elems;

public:
	SPStack2() = default;
	SPStack2(std::string elem) : elems({ std::move(elem) }) {}
	//SPStack2(std::initializer_list<std::string> ls) :elems{ ls } {}

	/* check if this is considered as universal reference not move reference!!*/
	void push(std::string&& elem);
	const std::string& top()  const&;
	std::string pop();
	bool empty() const { return elems.empty(); }

};

SPStack2(const char*)->SPStack2<std::string>;

const std::string& SPStack2<std::string>::top()  const&
{
	assert(!elems.empty());
	return elems.back();
}

void SPStack2<std::string>::push(std::string&& elem) 
{
	elems.push_back(std::forward<std::string>(elem));
}

std::string SPStack2<std::string>::pop()
{
	assert(!elems.empty());
	auto temp = std::move(elems.back());
	/* check if this is safe since we moved it before*/
	elems.pop_back();
	return temp;
}
