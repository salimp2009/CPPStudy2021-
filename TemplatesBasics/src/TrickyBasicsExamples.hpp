#pragma once

#include "Templpch.h"
#include "ArrayOverloads.hpp"

template<class T>
void printColl(const T& coll)
{
	typename T::const_iterator pos;
	typename T::const_iterator end(coll.end());

	for (pos = coll.begin(); pos != end; ++pos)
	{
		fmt::print("{0} \n", *pos);
	}
}


inline void TrickyBasics01()
{
	std::printf("\n------------TrickyBasics01------------\n");

	std::vector<int> vec1 = { 1,2,3,4 };
	printColl(vec1);
}

template<typename T, std::size_t N, std::size_t M>
constexpr bool lessArray(T(&a)[N], T(&b)[M])
{
	for (auto i = 0; i<N && i<M; ++i)
	{
		if (a[i] < b[i]) return true;
		if (b[i] < a[i]) return false;
	}

	return N < M;
}

inline void TrickyBasics_Arrays()
{
	std::printf("\n--TrickyBasics Arrays--\n");

	int x[] = { 1,2,3,4,5,6 };
	int y[] = { 1,2,3,4};
	fmt::print("less array: {0}\n", lessArray(x, y));

	char s1[] = "salim";
	char s2[] = "didem";
	fmt::print("less array: {0}\n", lessArray("didem", "salim"));
	fmt::print("less array: {0}\n", lessArray(s2, s1));

}

template<typename T1,  typename T2, typename T3>
void testArrays(int a1[7], int a2[], /*pointer by language rules*/
	int(&a3)[42],		 /*reference to array of known bounds*/
	int(&x0)[],			 /*reference to array of unknow bounds*/
	T1 x1,				 /*passing by value decays*/
	T2& x2, T3&& x3)	 /*passing by reference*/

{
	MyClass<decltype(a1)>::print();		/*goes T* overloaded version*/
	MyClass<decltype(a2)>::print();		/*goes T* overloaded version*/
	MyClass<decltype(a3)>::print();		/*goes T(&a)[n]; reference to array with known bounds*/
	MyClass<decltype(x0)>::print();		/*goes T(&a)[]; reference to array with unknown bounds*/
	MyClass<decltype(x1)>::print();
	MyClass<decltype(x2)>::print();
	MyClass<decltype(x3)>::print();
}

inline void ArrayOverloads_Test()
{
	std::printf("\n--Array Overloads--\n");
	int a[42] = { 0 };
	MyClass<decltype(a)>::print(); //goes to T[SZ]

	extern int x[];
	MyClass<decltype(x)>::print(); // goes to T[]

	testArrays(a, a, a, x, x, x, x);
}

/* used in the above example*/
int x[] = { 0,8,15 };

class BoolString
{
private:
	std::string value;
public:
	BoolString(std::string s) : value{ std::move(s) } {}

	template<typename T= std::string>
	T get() const { return value; }
};

/* specialization for the get member function
	it can only be defined not declare in the class boyd and define 
	but it can be also declared in the class body as well
*/
template<>
inline bool BoolString::get<bool>()const
{
	return value == "true" || value == "1" || value == "on";
}

template<unsigned long N>
void printBitSet(const std::bitset<N>& bs)
{
	/* if the we want explicitly specify to_string() member functions template parameter 
	we need use bs.template to_string<....> so that compiler knows < the operator is begining of a template*/
	//std::cout << bs.template to_string<char, std::char_traits<char>, std::allocator<char>>() << '\n';

	/* if not template paramater needed to specfiy in to_string ; just use regular*/
	//std::cout << bs.to_string() << '\n';

	/* it works with fmt as well*/
	fmt::print("bitset : to_string<>: {}\n", bs.template to_string<char, std::char_traits<char>, std::allocator<char>>());
	fmt::print("bitset : to_string<>: {}\n", bs.to_string());

}

inline void MemberFunctSpecialization_Test()
{
	std::printf("\n---Member Funct Specialization---\n");

	BoolString s1{ "salim" };
	fmt::print("boolstring get(): {}\n", s1.get());
	fmt::print("boolstring get<bool>(): {}\n", s1.get<bool>());
	s1 = BoolString{ "on" };
	fmt::print("boolstring get<bool>(): {}\n", s1.get<bool>());

	std::bitset<8> bs1(14);
	printBitSet(bs1);

}

template <typename T= long double>
constexpr T pi= static_cast<T>(3.1415926535897932385 );

inline void VariableTemplates_Test()
{
	std::printf("\n---Variable Templates---\n");

	fmt::print("pi<float>: {}\n", pi<float>);
	fmt::print("pi<double>: {}\n", pi<double>);

}
