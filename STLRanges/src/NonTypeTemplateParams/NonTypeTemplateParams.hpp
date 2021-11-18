#pragma once

// floating point NTTP are new in c++20
template<double T>
struct ADouble;


// literal class; a trivial default constexpr destructor, a constexpr constructor, a lambda or aggregate without a constructor
// see all requirements https://en.cppreference.com/w/cpp/named_req/LiteralType
template<typename CharT, std::size_t N>
struct fixed_string
{
	CharT data[N]{};

	constexpr fixed_string(const CharT (&strng)[N])
	{
		//since C++20 copy_n is constexpr
		std::copy_n(strng, N, data);
	}
};

template<fixed_string Str>
void printString()
{
	fmt::print("{}\n", Str.data);
}


template<fixed_string Str>
struct FixedStringContainer
{
	void print()
	{
		fmt::print("{}\n", Str.data);
	}
};

template<fixed_string Str>
struct FormatString
{
	static constexpr auto fmt = Str;
	static constexpr auto numArgs = std::ranges::count(fmt.data, '%');
	operator const auto* ()const { return fmt.data; }

};


template<typename... TArgs>
void print(auto formt, const TArgs&... args)
{
	static_assert(formt.numArgs == sizeof...(TArgs));
	std::printf(formt, args...);
}


template<fixed_string Str>
constexpr auto operator"" _fs()
{
	return FormatString<Str>{};
}


template<typename T, typename U>
constexpr bool plain_same_v = std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

template<typename T, typename U>
concept plain_same =plain_same_v<T, U> && plain_same_v<U, T>;

template<typename T>
constexpr static bool match(const char c)
{
	switch (c)
	{
		case 'd': return plain_same_v<int, T>;
		case 'c': return plain_same_v<char, T>;
		case 'f': return plain_same_v<double, T>;
		case 's': return (plain_same_v<char, std::remove_all_extents_t<T>> && std::is_array_v<T>)
					  || (plain_same_v<char*, std::remove_all_extents_t<T>> && std::is_pointer_v<T>) ;
	}

	return false;
}

template<typename T>
concept match_as = plain_same_v<int, T> || plain_same_v<char, T> || plain_same_v<double, T>
				|| (plain_same_v<char, std::remove_all_extents_t<T>> && std::is_array_v<T>)
				|| (plain_same_v<char*, std::remove_all_extents_t<T>> && std::is_pointer_v<T>);

template<int I, typename CharT>
constexpr auto get(const std::span<const CharT>& str)
{
	auto       start = std::begin(str);
	const auto end = std::end(str);

	for (int i = 0; i <= I; ++i) { 
	  
		start = std::ranges::find(start, end, '%');
		// iterate the start to see next % sign
		++start; 
	}

	return *start; 
}

template<typename CharT, typename... Ts>
constexpr bool IsMatching(std::span<const CharT> str)
{
	return[&]<std::size_t... I>(std::index_sequence<I...>)
	{
		return ((match<Ts>(get<I>(str))) && ...);
	}
	(std::make_index_sequence<sizeof...(Ts)>{});
}


template<typename... TArgs>
void print2(auto formt, const TArgs&... args)
{
	// check the number of args
	static_assert(formt.numArgs == sizeof...(TArgs));

	// check the types  of args vs specifiers
	static_assert
	(
		IsMatching<std::decay_t<decltype(formt.fmt.data[0])>, TArgs...>(formt.fmt.data)
	);

	std::printf(formt, args...);

}


template<typename... TArgs>
void print2(char* s, const TArgs... ts)
{
	std::printf(s, ts...);
}


template<typename... Ts>
constexpr bool always_false_v = false;

template<typename... TArgs>
void print2(const char* s, TArgs&&... ts)
{
	static_assert(always_false_v<TArgs...>, "Use user defined liter _fs");
}

// Examples from Rainer Grim's C++20 book
struct ClassTypes
{
	constexpr ClassTypes(int n) :value{ n } {}
	int value;
};

// class type NTTP
template <ClassTypes cl>
inline constexpr auto getClassType() { return cl; }

template<double val>
inline constexpr auto getDoubleValue() { return val; }

// all members have to public
template<int N>
class StringLiteral
{
public:
	constexpr StringLiteral(char const (&str)[N])
	{
		std::ranges::copy(str, data);
	}
	char data[N];
};

template<StringLiteral str>
class ClassTemplate 
{
public:
	using Type = typename decltype(StringLiteral(str));
};

template<StringLiteral str>
inline void FunctionTemplate()
{
	fmt::print("{}\n", str.data);
}



