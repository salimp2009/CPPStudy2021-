#pragma once

//#include "Templpch.h"

template<typename T>
struct MyClass;

template<typename T, std::size_t SZ>
struct MyClass<T[SZ]>
{
	static void print() { fmt::print("print(); array of known bounds T[{0}]\n", SZ); }
};

template<typename T, std::size_t SZ>
struct MyClass<T(&)[SZ]>
{
	static void print() { fmt::print("print(); ref to an array of known bound; T(&)[{0}]\n", SZ); }
};

template<typename T>
struct MyClass<T[]>
{
	static void print() { fmt::print("print(); array of unknown size T[]\n"); }
};

template<typename T>
struct MyClass<T(&)[]>
{
	static void print() { fmt::print("print();  ref to an array of unknown size; T(&)[]\n"); }
};

template<typename T>
struct MyClass<T*>
{
	static void print() { fmt::print("print(); pointer specialization T*\n"); }
};