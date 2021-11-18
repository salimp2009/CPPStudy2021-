#pragma once
#include "RangesPCH.hpp"

struct Address
{

	std::string city;
	std::string street;
	std::int32_t streetNo;

	constexpr auto operator<=>(const Address& rhs) const
	{
		if (const auto& cmp = city <=> rhs.city; cmp != 0)
		{
			return cmp;
		}
		else if (const auto& streetcmp = street <=> rhs.street; streetcmp != 0)
		{
			return streetcmp;
		}

		// sort street_no ascending
		return rhs.streetNo <=> streetNo;
	}

	bool operator==(const Address&) const = default;
};

struct Legacy
{
	int a;

	// pre c++20 these define weak_order
	bool operator==(const Legacy& rhs) const { return a==rhs.a; }
	bool operator<(const Legacy& rhs) const { return a< rhs.a; }
};

class LegacyWrap
{
public:
	LegacyWrap(int a, int b) :mA{ a }, mB{ b } {}

	std::weak_ordering operator<=>(const LegacyWrap&) const = default;

private:
	Legacy mA, mB;
};

struct B;


struct A
{
	// this needs to be const B so that it works correctly in c++20; works OK in c++17 but 
	// c++20 needs consistency therefore it gives the wrong output; given as an example !!!!
	bool operator==(B&) const { return true; }

	// The right way to write so it works OK!!!
	//bool operator==(const B&) const { return true; }

};

struct B
{
	bool operator==(const A&) const { return false; }
};

struct Basics
{
	int i;
	char c;
	float f;
	double d;
	auto operator<=>(const Basics&) const = default;
};

struct Arrays
{
	int		ai[1];
	char	ac[2];
	float	af[3];
	double	ad[2][2];

	auto operator<=>(const Arrays&) const = default;
};

struct Bases : Basics, Arrays
{
	auto operator<=>(const Bases&) const = default;
};


class MyInt2
{
public:
	// if explicit is used ; we need to extra comparision for any other value than the class since implicit conversion will be disabled
	constexpr explicit MyInt2(int val) : value{ val } {}

	auto operator<=>(const MyInt2&) const = default;
	
	constexpr auto operator<=>(const int& rhs) const
	{
		return value <=> rhs;
	}
private:
	int value;
};

class MyInt3
{
public:
	// if explicit is used ; we need to extra comparision for any other value than the class since implicit conversion will be disabled
	constexpr explicit MyInt3(int val) : value{ val } {}

	bool operator==(const MyInt3& rhs) const
	{
		std::puts("user defined ==");
		return value == rhs.value;
	}

	bool operator<(const MyInt3& rhs) const
	{
		std::puts("user defined <");
		return value < rhs.value;
	}

	auto operator<=>(const MyInt3&) const = default;

	constexpr auto operator<=>(const int& rhs) const
	{
		return value <=> rhs;
	}
private:
	int value;
};