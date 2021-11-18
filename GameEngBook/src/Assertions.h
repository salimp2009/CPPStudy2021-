#pragma once


#if ASSERTIONS_ENABLED	|| _DEBUG

#define debugBreak()	__debugbreak()

#define ASSERT(expr)\
	if (expr) { }\
	else \
	{\
		std::printf("%s failed FILE: %s, LINE: %i \n", #expr, __FILE__, __LINE__);\
		debugBreak();\
	}
#else

#define ASSERT(expr) //evaluates to nothing!!!


#endif //  
