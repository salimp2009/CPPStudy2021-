#pragma once

#include "Templpch.h"
#include <source_location>

template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

template<typename T>
concept UnSignedIntegral = Integral<T> && !SignedIntegral<T>;

template<typename T>
concept ArithmeticType = std::is_integral_v<T> || std::is_floating_point_v<T>;

inline void funct(UnSignedIntegral auto integ)
{
	fmt::print(" UnsignedIntegral: {}\n", integ);
}

inline void funct(SignedIntegral auto integ)
{
	fmt::print(" SignedIntegral: {}\n", integ);
}


template<typename T>
struct Other;

template<>
struct Other<std::vector<int>> {};

template<typename T>
concept TypeRequirement = requires
{
	/* defines that type T has a typedefinition value_type that refers the type inside the T; 
	   e.g vector<int>; int is the value_type*/
	typename T::value_type;	
	/* defines that Other is a type that can be instantiated with T */
	typename Other<T>;		
};

Other<std::vector<int>> oth1;

/* testing that std::vector<int> has a value_type typedef and Other can be instantiated with it*/
static_assert(TypeRequirement<std::vector<int>> );

TypeRequirement auto myval = std::vector<int>{ 1,2,3,4 };

template<typename T>
concept Equal = requires(T a, T b)
{
	{a == b} -> std::convertible_to<bool>;
	{a != b} -> std::convertible_to<bool>;
};

bool areEqual(Equal auto a, Equal auto b)
{
	return a == b;
}

struct WithoutEqual { bool operator==(const WithoutEqual& ) = delete; };

struct WithoutNotEqual { bool operator!=(const WithoutEqual& ) = delete; };


struct WithEqual
{
	bool operator!=(const WithEqual& b) { return false; }
	bool operator==(const WithEqual& b) { return true; }
};

/* both of them does not satisfy both conditions*/
static_assert(not Equal<WithoutEqual>);
static_assert(not Equal<WithoutNotEqual>);

/* satisfies both */
static_assert(Equal<WithEqual>);

template<typename T>
concept Ordering = Equal<T> &&
requires(T a, T b)
{
	{a <= b} ->std::convertible_to<bool>;
	{a <  b} ->std::convertible_to<bool>;
	{a >  b} ->std::convertible_to<bool>;
	{a >= b} ->std::convertible_to<bool>;
};

template<Ordering T>
constexpr T getSmaller(T a, T b)
{
	return (a < b ) ? a : b;
}

static_assert(getSmaller(true, false) == 0);
static_assert(getSmaller(1, 5) == 1);
static_assert(getSmaller('a', 'b') == 'a');

// Type trait for SemiRegular
template<typename T>
struct isSemiRegular : std::integral_constant<bool,
											  std::is_default_constructible_v<T> && std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> &&
											  std::is_move_constructible_v<T> && std::is_move_assignable_v<T> &&
											  std::is_destructible_v<T> && std::is_swappable_v<T> > {};

template<typename T>
concept SemiRegular = isSemiRegular<T>::value;

template<typename T>
concept Regular = Equal<T> && SemiRegular<T>;

template<typename T>
concept movable = std::is_object_v<T> && std::is_move_constructible_v<T> && std::assignable_from<T&, T> && std::swappable<T>;

template<typename T>
concept CopyAble = std::is_copy_constructible_v<T> && movable<T> &&
			       std::assignable_from<T&, T&> &&
				   std::assignable_from<T&, const T&> && std::assignable_from<T&, const T>;

template<typename T>
concept asSemiRegular = CopyAble<T> && std::default_initializable<T>;

template<typename T>
concept asRegular = asSemiRegular<T> && std::equality_comparable<T>;

// Used in ConceptExamples3.hpp
template<Regular T>
void behavesLikeAnInt(T)
{
	std::printf("behavesLikeAnInt\n");
	
	// Just quickly test for how std::source_location works; better to have the call location as an input!
	//std::source_location location = std::source_location::current();
	//fmt::print("function:{}, file:{}, line: {}\n", location.function_name(), location.file_name(), location.line());
}

template<std::regular T>
void behavesLikeAnInt2(T)
{
	std::printf("behavesLikeAnInt2\n");
}

struct EqualityComparable {};

bool operator==(const EqualityComparable&, const EqualityComparable&) { return true; }
//bool operator!=(const EqualityComparable&, const EqualityComparable&) { return false; }

struct NotEqualityComparable {};