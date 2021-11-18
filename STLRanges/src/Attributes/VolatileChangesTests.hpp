#pragma once
#include "RangesPCH.hpp"
#include "VolatileChanges.hpp"

inline void VolatileChanges()
{
	std::printf("\n- VolatileChanges-\n");

	dinasor = neck;		// volatile store; OK
	tail = dinasor;		// volatile load; OK

	// depreciated use cases for volatile ; access twice or once ????
	// MSVC gives no Warnings for Depreciated Volatile use; GCC & Clang gives warnings
	tail = dinasor = neck;
	dinasor += neck;

	// OK; not depreciated
	dinasor = dinasor+neck;
	//

	jurassic();

	trex(20, 30);
	
}


