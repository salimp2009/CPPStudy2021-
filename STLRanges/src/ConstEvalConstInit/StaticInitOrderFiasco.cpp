#include "RangesPCH.hpp"

// initialized in another translation unit using constinit;  
// to avoid Static Order Init problem; see staticOrderFiasco.cpp
extern int  staticValA2;

// Lazy Initialized function that has a static local value and return a reference to it  
// to avoid Static Order Init problem; see staticOrderFiasco.cpp 
int& staticA();
auto staticValB = staticA();

auto staticValB2 = staticValA2;