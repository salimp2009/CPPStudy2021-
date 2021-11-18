#pragma once

// Not compiling the custom format because of the MSVC bug in std::format_to referring to function Grow() in fmt_buffer_iterator!!
void metaprog_TypeManipulation();
void metaprog_MetaFunctvsRunTime();


template<typename T>
struct removeConst { using type = T; };

template<typename T>
struct removeConst<const T> { using type = T; };

template<typename T> 
struct removeReference { using type = T; };

template<typename T>
struct removeReference<T&> { using type = T; };

template<typename T>
struct removeReference<T&&> { using type = T; };

// this was not included in Cppreference TODO : if it is OK ???
//template<typename T>
//struct removeReference<T*> { using type = T; };

template<int a, int b>
struct ProductMeta { static const int value = a * b; };

// run time function
inline int PowerRunTime(int m, int n)
{
	int r = 1;
	for (int k = 1; k <= n; ++k) { r *= m; }
	return r;
}

inline constexpr int PowerHybrid(int m, int n)
{
	int r = 1;
	for (int k = 1; k <= n; ++k) { r *= m; }
	return r;
}

template<int m, int n>
struct Power
{
	static constexpr int value = m * Power<m, n - 1>::value;
};


template<int m>
struct Power<m, 0> { static constexpr int value = 1; };