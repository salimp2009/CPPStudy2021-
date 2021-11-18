#pragma once

#include "STLpch.h"
#include <any>
#include <charconv>

std::optional<int> asInt(const std::string& s)
{
	std::optional<int>ret;
	try
	{
		ret= std::stoi(s);
	}
	catch (...)
	{	
	}
	return ret;
}

inline void Optional_Basics()
{
	std::printf("\n-------------Optional_Basics C++17-------------\n");
	for (auto s : { "42", "077", "hello", "0x33" })
	{
		auto opt=asInt(s);
		//if (opt.has_value()) 
		//	std::cout << *opt << '\n';
		//else
		//	std::cout <<"cannot convert!!" << '\n';

		/* value_or() allocates memory therefore prefer the ALTERNATIVE below!*/
		//std::cout << "s: " << opt.value_or(0);

		// ALTERNATIVE to value or does not allocate memory
		fmt::print("optinional value: {}\n", (opt ? *opt : 0));
	}
	
}


inline void Variant_Basics()
{
	std::printf("\n-------------Variant_Basics C++17-------------\n");

	std::variant<int, std::string> var1{ "hi" };

	fmt::print("index of var1: {}\n", var1.index());

	fmt::print("value of var1: {}\n", std::get<1>(var1));

	std::variant<std::monostate, int, int, std::string> var2;
	fmt::print("index of var2: {}\n", var2.index());

	std::variant<std::complex<double>> var3{ std::in_place_index<0>, 12.2, 2.25 };
	fmt::print("value of var3: {}\n", std::get<0>(var3));

	std::variant<std::complex<double>> var4;
	var4.emplace<0>(12.2, 35.45);

	fmt::print("value of var4: {}\n", std::get<0>(var4));

	std::visit([](auto&& val) {fmt::print("val : {}\n", val); }, var1);
	var1 = 1;
	std::visit([](auto&& val) {fmt::print("revised val : {}\n", val); }, var1);
	var1 = 25;
	//std::visit([](auto&& val)
	//	{
	//		/* restores the default of the current alternative; e.g. it becomes empty string or empty int*/
	//		val = std::remove_reference_t<decltype(val)>{};
	//		fmt::print("revised val : {}\n", val);
	//	}, var1);

	//fmt::print("index of var1: {}\n", var1.index());

	auto doubleVal = [](auto& val)
	{
		/* since the val is a reference if we used std::is_same it will the address of the variable 
		   but if we check convertible than it compares the value
		*/
		if constexpr (std::is_convertible_v<decltype(val), std::string>)
		{
			val += val;
			fmt::print("val : {}\n", val);
		}
		else
		{
			val *= 2;
			fmt::print("val : {}\n", val);
		}
	};

	std::visit(doubleVal, var1);
}

inline void StdAny_Basics()
{
	std::printf("\n-------------std::any-------------\n");
	/* Avoid using std::any; bring extra memory allocations for a big type
	  and the size is quite big!!!!
	*/
	std::any aVal(12);
		
	if (aVal.has_value() && aVal.type()==typeid(int))
	{
		fmt::print("any val : {}\n", std::any_cast<int>(aVal));
	}
	
}

inline void StdByte_Basics()
{
	std::printf("\n-------------std::byte------------\n");

	/* Only binary operations allowed*/
	std::byte b1{0x3f};
	std::byte b2{ 0b1111'0000 };
	std::byte b3[4]{ b1, b2, std::byte{1} }; // Any array of std::byte , last value becomes zero since nothing is passed 

	std::cout <<"b1: "<<std::to_integer<int>(b1) << '\n';

	if (b1 == b3[0])
	{
		b1 <<= 1;
	}

	std::cout << "b1 revised: " << std::to_integer<int>(b1)<<'\n';
	std::cout << "sizeof b1: "<<sizeof b1 << '\n';

}

inline void fromChars_Example()
{
	std::printf("\n-----fromChars----\n");

	const char* str = "12 monkeys";
	int value;
	std::from_chars_result res = std::from_chars(str, str + 10, value, 10); // the last 10 is the base (16 for hex, 2 for binary..)
	
	/* no implicit coversion std::errc to bool therefre (!res.ec) will not compiles*/
	if (res.ec != std::errc{})
	{
		std::printf("from chars conversion failed!\n");
	}

	fmt::print("value: {0}, res.ptr: {1}\n", value, (res.ptr - str));

	const char* str2 = "101001";
	int value2;
	res = std::from_chars(str2, str2 + 6, value2, 2); // base is 2 ; binary 

	if (res.ec == std::errc{})
	{
		fmt::print("value(101001): {0}, res.ptr: {1}\n", value2, (res.ptr - str2));
	}
}


inline void toChars_Example()
{
	std::printf("\n-----toChars----\n");

	int value = 42;
	char buffer[std::numeric_limits<int>::digits10+2];
	
	std::to_chars_result res = std::to_chars(buffer, buffer + std::size(buffer), value);

	if (res.ec == std::errc{})
	{
		/* add a trailing zero for termination of string if using char[] 
		otherwise garbage characters will be displayed because it does not know where to stop 
		Better to use std::array<char, ...> then no need to add a trailing zero
		*/
		*res.ptr = '\0';
		fmt::print("value: {0}\n", buffer);
	}
}


inline void fromChars_Example2()
{
	std::printf("\n-----fromChars Example 2----\n");

	/* the type of array is deduced as const char*/
	const std::array<char, 15> str = { "12345678901234" };
	/* if int is used we get an out of range error; the error does not throw or allocate memory*/
	uint64_t value=0;
	const auto res = std::from_chars(str.data(), str.data() + str.size(), value);
	if (res.ec == std::errc{})
	{
		fmt::print("res using std::array: {0}, distance = {1}\n", value, res.ptr-str.data());
	}
	else if (res.ec == std::errc::invalid_argument)
	{
		fmt::print("invalid argument!\n");
	}
	else if (res.ec == std::errc::result_out_of_range)
	{
		fmt::print("result_out_of_range!, res.ptr distance: {0}\n", res.ptr -str.data());
	}
}


inline void fromChars_floatingPoint()
{
	std::printf("\n-----fromChars Floating Point----\n");
	const std::array<char, std::numeric_limits<float>::digits> str = { "-67.1235" };
	double value = 0;

	/* Format used to test different setting and types ; general is combinatin of fixed and scientific chose automatically*/
	const auto format = std::chars_format::general;  
	//const auto format = std::chars_format::fixed;
	//const auto format = std::chars_format::scientific;
	//const auto format = std::chars_format::hex;
	
	const auto res = std::from_chars(str.data(), str.data() + str.size(), value, format);

	if (res.ec == std::errc{})
	{
		fmt::print("res using std::array: {0}, distance = {1}\n", value, res.ptr - str.data());
	}
	else if (res.ec == std::errc::invalid_argument)
	{
		fmt::print("invalid argument!\n");
	}
	else if (res.ec == std::errc::result_out_of_range)
	{
		fmt::print("result_out_of_range!, res.ptr distance: {0}\n", res.ptr - str.data());
	}
}

inline void toChars_Integers()
{
	std::printf("\n-----toChars_Integers----\n");

	std::array<char, std::numeric_limits<int>::digits10 + 2> str;

	const int value = -198619;

	auto res = std::to_chars(str.data(), str.data() + str.size(), value, 10);

	if (res.ec == std::errc{})
	{
		/* if the value is too large then VS Studio will throw exception in Debug mode but not in release check ; VS Studio has array bound check in debug mode!
		   if the string_view is used there is no need to do this 
		*/
		*res.ptr = '\0';
		fmt::print("res to_chars: {0}, characters = {1}\n", str.data(), res.ptr - str.data());

		/*Alternative to above string_view can be used without adding the null character at the end but to be safe it is better to add
		  NOTE: the null character can be added by initializing the array with value zero
		  NOTE: For c++20; string_view(str.data(), res.ptr) is enough
		*/
		fmt::print("res to_chars: {0}, characters = {1}\n", std::string_view(str.data(), res.ptr - str.data()), res.ptr - str.data());
	}
	else
	{
		fmt::print("value too large to fit in!");
	}

}
