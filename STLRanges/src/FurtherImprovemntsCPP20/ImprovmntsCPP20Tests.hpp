#pragma once
#include "RangesPCH.hpp"
#include "ImprovementsCPP20.hpp"
#include <iostream>

inline void RangeBasedForLoopInitialize()
{
	std::printf("\n- RangeBasedForLoopInitialize-\n");

	for (auto vec = std::vector{ 1,2,3,4,5 }; const auto & elem :vec)
	{
		std::printf("%d ", elem);
	}
	std::puts(" ");

	// classic way initializing the vector
	for (std::vector<int> vec2{ 1,2,3,4,5 }; const auto & elem :vec2)
	{
		std::printf("%d ", elem);
	}
	std::puts(" ");

	// initializer list example but initializer list can be written directly;  e.g for( auto elem : {1,2,3,4,5})
	for (auto InitList = { 100,200,300,400,500 }; auto elem:InitList)
	{
		std::printf("%d ", elem);
	}
	std::puts(" ");
	
}

inline void VirtualConstExprFunction()
{
	std::printf("\n-VirtualConstExprFunction-\n");

	constexpr X2 x2;
	constexpr X3 x3;
	constexpr auto val = x2.f();
	static_assert(x2.f() == 2);
	
	X1* xx1 = new X4;
	std::printf("x1: %d \n", xx1->f());
	
	X4 x4;
	X1& xx2 = x4;
	std::printf("xx2: %d \n", xx2.f());

	delete xx1;
}

inline void Char8Strings()
{
	std::printf("\n-Char8Strings-\n");

	// char8_t is 8 bits (1 byte); large enough to hold UTF-8 code; it has same size, signedness and alignment of unsigned char
	// char is a byte; generally a byte is 8 bits but it is platform defined there the bits of a byte and therefore bits of a char is not defined
	// string is an alias to basic_string<char>
	// So the new additional types ; std::u8string , std::basic_strin<char8_t>  e.g (string literal); u8"Hello World"
	// u8; is the UTF-8 char is a character literal ; https://en.cppreference.com/w/cpp/language/character_literal

	// NOTE: char8_t might not be supported everywhere; GCC & Clang does not support and also maynot work correctly for every UTF-8 types !!!!

	const char8_t* char8Str = u8"Hello World";
	std::basic_string<char8_t> char8String = u8"Hello World";
	std::u8string char8String2 = u8"Hello World";

	char8String2 += u8".";

	std::printf("char8String.size(): %zd \n", char8String.size());
	std::printf("char8String2.size(): %zd \n", char8String2.size());

	char8String2.replace(0, 5, u8"Hi");
	
	std::printf("char8String2.size(): %zd \n", char8String2.size());

}

inline void usingEnumLocalScope()
{
	std::printf("\n- usingEnumLocalScope-\n");
	
	fmt::print("toString(Color::Red): {} \n", toString(ColorMe::Red));
}

inline void bitFieldInitialization()
{
	std::printf("\n- bitFieldInitialization-\n");

	std::printf("sizeof (Class11): %zd \n", sizeof(Class11));
	std::printf("sizeof (BitField20): %zd \n", sizeof(BitField20));
}

inline void IsConstEval()
{
	std::printf("\n-IsConstEval-\n");

	constexpr double kilo1 = power(10.0, 3);
 
	static_assert(kilo1 == 1000);

	std::printf("kilo1: %f \n", kilo1);

	constexpr int n = 3;
	constexpr double kilo2 = power(10.0, n);
	
	static_assert(kilo2 == 1000);

	std::printf("kilo2: %f \n", kilo2);

	int y{};
	const auto result1 = std::is_constant_evaluated() ? y : 1;
	//static_assert(result1 != y);
	std::printf("result1 : %d\n", result1);
	
	const auto result2 = std::is_constant_evaluated() ? 5 : 0;
	static_assert(result2 == 5);
	std::printf("result1 : %d\n", result2);
}

inline void logSourceLocation()
{
	std::printf("\n-logSourceLocation-\n");
	log("logging my message");
}

inline void toAddressTest()
{
	std::printf("\n-toAddressTest-\n");

	int myInt{ 5 };
	int* myIntPtr{ &myInt };

	// the address of the myInt
	fmt::print("address of myInt: {} \n",(void*) &myInt);
	
	// address of the pointer that points to myInt (not the address of myInt; the value of the pointer is the address of the myInt)
	fmt::print("address of myIntPtr: {} \n", (void*)&myIntPtr);


	// the address of the myInt
	std::cout << "address of myInt: " << &myInt << '\n';
	

	// address of the pointer that points to myInt (not the address of myInt; the value of the pointer is the address of the myInt)
	std::cout << "address of myIntPtr: " << &myIntPtr <<'\n';

	// address of object that pointer points to (address myInt)
	std::cout << "to_address(myIntPtr): " << std::to_address(myIntPtr)<< '\n';

	auto uniquePtr = std::make_unique<int>(5);
	auto uniquePtr2 = std::make_unique<int>(5);

	std::cout << "to_address(uniqueptr): " << std::to_address(uniquePtr) << '\n';
	std::cout << "to_address(uniqueptr.get()): " << std::to_address(uniquePtr.get()) << '\n';

	std::cout << "to_address(uniqueptr2): " << std::to_address(uniquePtr2) << '\n';
	std::cout << "to_address(uniqueptr2.get()): " << std::to_address(uniquePtr2.get()) << '\n';
}









