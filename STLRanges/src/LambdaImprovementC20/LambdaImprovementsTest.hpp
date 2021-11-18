#pragma once
#include "RangesPCH.hpp"
#include "LambdaImprovements.hpp"


inline void LambdaCapturesInClass()
{
	std::printf("\n-LambdaCapturesInClass-\n");

	SomeClass somecls1;
	fmt::print("{}", somecls1.getTwiceValue());
}

inline void DefaultConstructibleLambdas()
{
	std::printf("\n-DefaultConstructibleLambdas-\n");

	const Book effectiveCpp{ "Effective C++", "978-3-16-148410-0" };
	const Book functProgCpp{"Functional Programming in C++", "978-3-20-148410-0"};

	const Price normal{ 34.95 };
	const Price reduced{ 24.95 };

	// After C++20
	MapBookSortedbyISBN<Price> book2Price{ {effectiveCpp, normal}, {functProgCpp, reduced} };

	// Before C++20; gives compiler error if compiled with C++20 ; OK if compiled with C++17
	// MapBookSortedbyISBN<Price> book3Price( {effectiveCpp, normal}, {functProgCpp, reduced}, compare );
	for (const auto& [book, price] : book2Price)
	{
		fmt::print("book name: {}, ISBN: {}, Price: {}\n", book.title, book.isbn, price.amount);
	}
}

inline void LambdasInGenericCode()
{
	std::printf("\n-LambdasInGenericCode-\n");

	const Book effectiveCpp{ "Effective C++", "978-3-16-148410-0" };
	const Book functProgCpp{ "Functional Programming in C++", "978-3-20-148410-0" };

	const Price normal{ 34.95 };
	const Price reduced{ 24.95 };

	MapSortedbyISBN < Book, Price> book2Price = { {effectiveCpp, normal}, {functProgCpp, reduced} };

	for (const auto& [book, price] : book2Price)
	{
		fmt::print("book name: {}, ISBN: {}, Price: {}\n", book.title, book.isbn, price.amount);
	}

	// Testing NoISBN and concept
	const BookNoISBN effectiveCpp2{ "Effective C++" };
	const BookNoISBN functProgCpp2{ "Functional Programming in C++" };

	// Tested with NoIsBN does not compile as expected 
	//MapSortedbyISBN < BookNoISBN, Price> book3Price = { {effectiveCpp2, normal}, {functProgCpp2, reduced} };

	const Magazine ix{ "iX", "978-3-16-148410-0" };
	const Magazine overload{ "overload", "978-3-20-148410-0" };

	MapSortedbyISBN<Magazine, Price> magazine2Price{ {ix, reduced}, {overload, normal} };

	for (const auto& [magazine, price] : magazine2Price)
	{
		fmt::print("magazine name: {}, ISBN: {}, Price: {}\n",magazine.name, magazine.isbn, price.amount);
	}

	// Testing HasISBN concept
	static_assert(HasISBN<Magazine>);
	static_assert(!HasISBN<BookNoISBN>);
}


// Does not compile with MSVC; MSVC Bug When there 2 variadic arguments in Lambda Capture ; it does not work; compİLES WİTH gcc / clang
inline void LambdasWithVariadicCapture()
{
	using namespace std::string_literals;
	std::printf("\n- LambdasWithVariadicCapture-\n");
#if not defined(_MSC_VER)
	auto steeringLogger = getNamedLogger("steering");
	steeringLogger("angle", 90);
#endif
}

inline void GenericLambdasWithConcepts()
{
	std::printf("\n- GenericLambdasWithConcepts-\n");
	using namespace std::string_literals;

	// incomplete due to MSVC bug with more than one variadic arguments in lambdas
}

inline void ClassLambdasWithVariableCapture()
{
	std::printf("\n-ClassLambdasWithVariableCapture-\n");

	// when the lambda captures by [=] it implicity captures this and + C++20 gives you of warning of depreciated but MSVC does not
	// this is undefine behaviour since we are returning a lambda that return the a variable of struct that goes out of scope when we call better to capture [*this]
	//auto lambdaFunct = makeLambda();
	//lambdaFunct();

	// this one use capture [*this] therefore lambda return a copy and it works correctly as expected
	auto lambdaFunct2 = makeLambda2();
	lambdaFunct2();

}





