#pragma once
#include "RangesPCH.hpp"

// Depreciated cases for volatile

int neck, tail;
volatile int dinasor;

struct amber {};

// depreciated volatile use in c++20; MSVC gives no warning
volatile struct amber jurassic() { return {}; }
	
// data is volatile ; not the pointer
void fly(volatile struct pretausorious* dynos) { }

// depreciated volatile use in functions; MSVC gives no warning!!
void trex(volatile short leftarm, volatile short rightarm) {}

// Volatile Description from Rainer Grimm C++20 Book
/*
	"volatile is typically used to denote objects that can change independently of the regular
	program flow. These are, for example, objects in embedded programming that represent an
	external device (memory-mapped I/O). Because these objects can change independently of
	the regular program flow and their value is directly written to main memory, no optimized
	storing in caches takes place. In other words, volatile avoids aggressive optimization
	and has no multithreading semantics"
*/

