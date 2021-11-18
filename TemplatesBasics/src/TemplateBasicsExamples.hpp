#pragma once

#include "Templpch.h"

#include "Max5.hpp"
#include "EnableIf_Utils.hpp"
#include "NontypeTemplateParams.hpp"

template<typename T>
T max(T a, T b)
{
	return b < a ? a : b;
}

template<>
std::string_view max(std::string_view a, std::string_view b)
{
	return b < a ? a : b;
}

// decay_T is used to get the type only; any && or const will be removed
// declval can also be used if the passed type dont have default constructor; 
// best way to deduce the Return type decltype(auto)
// auto always decays !!!!
template<typename T1, typename T2>
auto Max2(T1 a, T2 b)->std::decay_t<decltype(b < a ? a : b)>
{
	return b < a ? a : b;
}

// auto type deduction for function default values will not work 
//if the template type does not have a defaut value; this works
template<typename T = std::string_view>
void f(T = "");

template<typename T>
void f(T a) { std::cout << a << '\n'; }


int max3(int a, int b)
{
	return b < a ? a : b;
}

template<typename T>
T max3(T a, T b)
{
	return b < a ? a : b;
}

template<typename T>
T max4(T a, T b)

{
	return b < a ? a : b;
}


inline void FunctionTemplates_Basics()
{

	[[maybe_unused]] auto myval1 = GetInt(5);
	//std::cout << myIntval << '\n';

	auto m5 = ::max5(100, 42, 68);
	printf("max5: %d \n", m5);

	const char* mychar1 = "salim";
	const char* mychar2 = "didem";
	const char* mychar3 = "sema";

	/** the function returns the address of temp created in the function
		gives warning that it returns a temp value; better to use universal forwarding and not to mix overloads
	*/
	//auto&& m6 = ::max5(mychar1, mychar2, mychar3);

	//printf("max6: %s \n", m6);

	int a = 4;
	int b = 5;
	auto m1 = ::max4(a, b);			// expected to use template T version	

	std::string s3 = "didem";
	std::string s4 = "salim";
	auto m2 = ::max4(s3, s4);			// expected to use template T version ; negative frm strcmp so max4 return salim

	int* p1 = &a;
	int* p2 = &b;
	auto m3 = ::max4(p1, p2);		// expected to use T* template

	const char* x = "demir";
	const char* y = "sema";
	auto m4 = ::max4(x, y);			// expected to use T* template



	::max3(7, 42);			// expected int from non template
	::max3(7.0, 42.0);		// expected double from template
	::max3('a', 'b');		// expected char from template
	::max<>(7, 42);			// expected int from template
	::max3<double>(7, 42);	// expected double from template 
	::max3('a', 42);		// expected int implicit conversion from non template; template version does not allow unless used with enable if 


	std::cout << Max2(4.2, 7.9) << '\n';

	f(1);
	f();

	std::cout << ::max(10, 20) << '\n';
	std::cout << ::max("salim", "didem") << '\n';

	std::string s1{ "didem" };
	std::string s2{ "demir" };

	std::cout << ::max<std::string_view>(s1, s2) << '\n';

	// Raw string example ; R"(SOME TEXT)"
	// To include )" inside the string add delimiters; e.g. R"nc(SOME TEXT)nc"
	std::cout << R"([a()
					b"])" << '\n';

}

inline void NonTemplateParams_Basics()
{
	Message<42> mymsg1;
	mymsg1.printMsg();

	static const char cstrng[] = "saSASD";
	Message<cstrng> mymsg2;
	mymsg2.printMsg();

	auto  c = 42;
}

