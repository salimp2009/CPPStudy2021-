#pragma once
#include "RangesPCH.hpp"

constexpr int maxElements()
{
	std::vector myVec = { 7,5,3,4 };
	std::sort(myVec.begin(), myVec.end());
	return myVec.back();
}


inline void ConstExpr_Vector()
{
	std::printf("\n---constexpr_vector---\n");

	// intellisense error but it works ; checked in compilerexplorer; note does not work in GCC 11.1 and Clang 12
	constexpr int value = maxElements();
	std::printf("%d", value);
}


inline void makeArray()
{
	std::printf("\n---makeArray---\n");

	// creates const char* array[]; each element of the array is a c-string
	auto arr1 = std::to_array({ "testing an array" });
	fmt::print("c-string: {}, element count: {}\n", arr1, arr1.size());

	// creates a char array[] a c-string; each character is a seperate element of the array!
	auto arr1a = std::to_array("testing an array");
	fmt::print("char array: {}, element count: {} \n\n", arr1a, arr1a.size());

	auto arr2 = std::array{ 1, 2, 3, 4, 5 };
	fmt::print("array: {}, element count: {}\n", arr2, arr2.size());
	
	auto arr2a = std::to_array({ 1,2,3,4,5 });
	fmt::print("to_array: {}, element count: {}\n\n", arr2a, arr2a.size());
	
	auto arr3 = std::to_array<double>({ 0,1,3 });
	fmt::print("to_array<double>: {}, typeid: {}, size: {}\n\n", arr3, typeid(arr3[0]).name(), arr3.size());

	auto arr4 = std::to_array<std::pair<int, double>>({ { 3,4.0 }, {5,6.2}, { 25,10.25 } });
	
	fmt::print("to_array<pair>: {}, typeid: {}, size: {}\n\n", arr4, typeid(arr4[0]).name(), arr4.size());

}

inline void makeShared()
{
	std::printf("\n---makeShared---\n");

	// before c++20 ; this would be ; std::shared_ptr<double[]> dblPtr(new double[1024])
	// std::make_shared do not allow custom deleters and 
	// if the user defined class uses class specific ::new overload then make_shared does not use it; prefer the old way in that case
	// make_shared will prevent leaks in case there is an exepception thrown while it is not fully constructed
	std::shared_ptr<double[]> dblPtr = std::make_shared<double[]>(1024);
	std::shared_ptr<double[]> dblPtr2 = std::make_shared<double[]>(100, 20.0); // 100 elements with value zero

	fmt::print("dblPtr[0]: {}\n", dblPtr[0]);
	fmt::print("dblPtr2[0]: {}\n", dblPtr2[0]);

}

inline void eraseif()
{
	std::printf("\n--- eraseif---\n");
	//works for STL containers and all STL like container types(not checked!)
	std::vector<int> vec1{ 1,2,3,4,5,6,7,8,9,10 };
	auto noOfDeletedElems = std::erase_if(vec1, [](auto&& elem) { return elem % 2 == 0; });
	fmt::print("revised vec1: {}, number of deleted elems: {}\n", vec1, noOfDeletedElems);

	std::string testStrng = "this has a big E";
	noOfDeletedElems = std::erase(testStrng, 'E');
	fmt::print("revised string: {}, number of deleted elems: {}\n", testStrng, noOfDeletedElems);

	std::deque deq{ 1,2,3,4,5,6,7,8,9 };
	noOfDeletedElems = std::erase_if(deq, [](auto elem) {return elem % 2 == 1; });
	fmt::print("revised deque: {}, number of deleted elems: {}\n", deq, noOfDeletedElems);

	std::list lst1{ 11,22,33,44,55,77,65 };
	noOfDeletedElems = std::erase(lst1, 22);
	fmt::print("revised list: {}, number of deleted elems: {}\n", lst1, noOfDeletedElems);

}

inline void checkwithContains()
{
	std::printf("\n--- checkwithContains---\n");

	auto checkElements = []<class Container, typename... T>(const Container& container, T&&... values)
	{
		return (container.contains(values) && ...);
	};

	std::set<int> mySet{ 1,2,3,4,5,6,7,8,9,10 };

	auto result = checkElements(mySet, 1);
	fmt::print("contains result: {}\n", result);
	
	// contains check if the key exits ; does not look for the value for the unordered / ordered maps, sets...
	std::unordered_map<int, std::string> myMap = { {49, "Salim"}, {48, "Didem"}, {16, "Demir"} };
	result = checkElements(myMap, 49);
	fmt::print("contains result: {}\n", result);

	result = checkElements(myMap, 57);
	fmt::print("contains result: {}\n", result);

	result = checkElements(myMap, 49, 48);
	fmt::print("contains result: {}\n", result);
}

inline void stringPrefixSuffixCheck()
{
	std::printf("\n--stringPrefixSuffixCheck--\n");

	auto startsWith = [](std::string_view str, auto prefix)
	{
		fmt::print("{} starts_with [{}]: {}\n",str, prefix, str.starts_with(prefix));
	};

	auto endsWith = [](std::string_view str, auto prefix)
	{
		fmt::print("{} ends_with [{}]: {}\n", str, prefix, str.ends_with(prefix));
	};

	std::string str1 = "HelloWorld";
	startsWith(str1, std::basic_string_view("Hello"));
	startsWith(str1, std::basic_string_view("World"));
	startsWith(str1, 'H');
	
	endsWith(str1, std::basic_string_view("Hello"));
	endsWith(str1, std::basic_string_view("World"));
	endsWith(str1, 'H');
	endsWith(str1, 'd');
}

inline void safeIntegerComparision()
{
	std::printf("\n--stringPrefixSuffixCheck--\n");

	int x = -3;
	unsigned int y = 7;

	// give result as false since x is converted unsigned int and it is way bigger than 7 
	// since there is 1 at the left most of the binary to express negative number when converted to unsigned the number becomes 2^32
	//fmt::print("-3<7: {} ", x,y);  
	fmt::print("-3<7: {}\n", std::cmp_less(x,y));
	fmt::print("-3<=7: {}\n", std::cmp_less_equal(x,y));
	fmt::print("-3!=7: {}\n", std::cmp_not_equal(x,y));
	fmt::print("-3==7: {}\n", std::cmp_equal(x,y));
	fmt::print("-3>7: {}\n", std::cmp_greater(x,y));
	fmt::print("-3>=7: {}\n", std::cmp_greater_equal(x,y));
	
	double xd = -3.5;
	unsigned int y2 = 7;
	// y2 is converted to floating point / double to compare and then converted to int to be represented as "bool"
	auto result = xd < y2;
	fmt::print("-3.5<7 : {}\n", result);

	// this will not work since std::cmp_less expectes integer
	//result = std::cmp_less(xd, y2);
}


inline void mathConstants()
{
	std::printf("\n--mathConstants--\n");
	
	// the math constant are constexpr constant values; see https://en.cppreference.com/w/cpp/numeric/constants
	// "STL specializes mathematical constant variable templates for all floating-point types (i.e. float, double and long double)"

	fmt::print("math const  e_v<double>  : {}\n", std::numbers::e_v<double>);
	fmt::print("math const e_v<float> : {}\n", std::numbers::e_v<float>);
	// e=e_v<double>
	fmt::print("math const e : {}\n", std::numbers::e);
	
	fmt::print("math const log2e_v<double> : {}\n", std::numbers::log2e_v<double>);
	fmt::print("math const log2e_v<float> : {}\n", std::numbers::log2e_v<float>);
	fmt::print("math const log2e : {}\n", std::numbers::log2e);
}

inline void midPointLerp()
{
	std::printf("\n--midPointLerp--\n");

	fmt::print("std::midpoint(10,20): {}\n", std::midpoint(10, 20));
	fmt::print("std::midpoint(15.23, 200.25): {}\n", std::midpoint(15.23, 200.25));

	std::uint32_t a = std::numeric_limits<std::uint32_t>::max();
	std::uint32_t b = std::numeric_limits<std::uint32_t>::max() - 2;

	fmt::print("a: {0}, b: {1}\n", a, b);
	fmt::print("(a + b)/2 (expected overflow): {}\n", (a + b) / 2);
	fmt::print("std::midpoint(a,b) (Not expected overflow): {}\n", std::midpoint(a, b));

	for (auto v : { 0.0, 0.1,0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 })
	{
		fmt::print("lerp(10, 20, {0}) : {1}\n", v, std::lerp(10, 20, v));
	}
}

inline void getEndian()
{
	std::printf("\n--getEndian-\n");

	if constexpr (std::endian::native == std::endian::big)
	{
		puts("std::endian::native == big endian");
	}
	else if constexpr (std::endian::native == std::endian::little)
	{
		puts("std::endian::native == litte endian");
	}
}

inline void bitOperations()
{
	std::printf("\n--bitOperations-\n");

	auto result = std::has_single_bit(5u);
	fmt::print("std::has_single_bit(5u): {}\n", result);

	const std::uint8_t testValue = 0b0011'0010;
	fmt::print("test value: binary:{}, dec: {}\n", std::bitset<8>(testValue), testValue);
	fmt::print("std::has_single_bit: {}\n", std::has_single_bit(testValue));
	fmt::print("std::std::bit_ceil: {}\n", std::bitset<8>(std::bit_ceil(testValue)));
	fmt::print("std::bit_floor: {}\n", std::bitset<8>(std::bit_floor(testValue)));

	//the smallest number of bits needed to represent the given value
	fmt::print("bit_width(testValue): {}\n", std::bit_width(testValue));
	fmt::print("bit_width(5u): {}\n", std::bit_width(5u));
	fmt::print("bit_width(testValue): {}\n", std::rotl(testValue,0));
	fmt::print("std::rotl(testValue,1): binary: {}, dec: {}\n", std::bitset<8>(std::rotl(testValue, 1)), std::rotl(testValue, 1));
	fmt::print("std::rotl(testValue,4): binary: {}, dec: {}\n", std::bitset<8>(std::rotl(testValue,4)), std::rotl(testValue, 4));
	fmt::print("std::rotl(testValue,3): binary: {}, dec: {}\n", std::bitset<8>(std::rotl(testValue,3)), std::rotl(testValue, 3));
	fmt::print("std::rotl(testValue,5): binary: {}, dec: {}\n", std::bitset<8>(std::rotl(testValue,5)), std::rotl(testValue, 5));

	//the number of consecutive 1 bits, starting from the most significant bit
	fmt::print("std::countl_one(testValue): {}\n", std::countl_one(testValue));
	//the number of consecutive 0 bits, starting from the most significant bit
	fmt::print("std::countl_zero(testValue: {}\n", std::countl_zero(testValue));
	
		fmt::print("std::countr_one(testValue): {}\n", std::countr_one(testValue));
	fmt::print("std::countr_one(testValue): {}\n", std::countr_one(testValue));

}






