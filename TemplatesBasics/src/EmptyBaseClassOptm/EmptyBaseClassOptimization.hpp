#pragma once

// an empty class has a size pf 1 byte so that pointer arithmetics can be implemented on the address
class EmptyClass{};

// type alias dont make a class nonempty

class Empty
{
	using Int = int; 
};

// if EBCO is implemented by the compiler all derived class and base class has size=1 byte
class EmptyToo : public Empty
{

};

class EmptyThree : public EmptyToo
{

};
