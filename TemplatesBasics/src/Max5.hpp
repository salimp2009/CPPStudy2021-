#pragma once


template<typename T>
const T& max5(const T& a, const T& b)
{
	return (b < a) ? a : b;
}

// try not mix when overloading the function; if it is called by reference use by ref in other one 
// if some overloads use ref other value it may create unexprected behaviour; not happened in this case but it might

// use references in templates only for expensive types otherwise better to use simple and by value; use std::string_view for string
const char* max5(const char* a, const char* b)
{
	return std::strcmp(b , a) < 0 ? a:b;
}

template<typename T>
const T& max5(const T& a, const T& b, const T& c)
{

	return max5(max5(a, b), c);
}
