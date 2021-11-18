#pragma once
#include "RangesPCH.hpp"


struct MyInt
{
	int value;
	explicit constexpr MyInt(int val) : value{ val } {}
	bool  operator==(const MyInt&) const = default;
	auto operator<=>(const MyInt& rhs) const { return value <=> rhs.value; }
};

struct MyDouble
{
	double value;
	explicit constexpr MyDouble(double val) : value{ val } {}
	auto  operator<=>(const MyDouble&) const = default;
};

struct VectorPointer
{
	std::vector<int>* pointertoVector;
	auto operator<=>(const VectorPointer&) const = default;
};



class String
{
public:
	template<std::size_t N>
	explicit String(const char(&src)[N]) : mData{ src }, mLen{ N } {}

	const char* begin() const { return mData; }
	const char* end() const { return mData + mLen; }

	auto operator<=>(const String& other) const
	{
		return Compare(*this, other);
	}

	bool operator==(const String& other)
	{
		if (mLen != other.mLen) { return false; }
		return Compare(*this, other)==0;
	}

private:
	const char* mData;
	std::size_t mLen;

	// example in book used weak_ordering but std::std::lexicographical_compare is case sensitive by default so h !=H therefore revised to strong ordering
	static std::strong_ordering Compare(const String& a, const String& b);
};


std::strong_ordering String::Compare(const String& a, const String& b)
{
	
		if (a.mLen == b.mLen &&
			std::equal(a.begin(), a.end(), b.begin(), b.end())) 
		{
			return std::strong_ordering::equal;
		}

		// lex_compare uses less<> funct by default so if it is true, a<b==true
		if (std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end())) 
		{
			return	std::strong_ordering::less;				
		}

		return std::strong_ordering::greater;
}

struct Strong 
{ 
	std::strong_ordering operator<=>(const Strong &) const = default;
};

struct Weak 
{
  std::weak_ordering operator<=>(const Weak &) const = default;
};

struct Partial 
{
  std::partial_ordering operator<=>(const Partial &) const = default;
};

struct StrongWeakPartial
{
	Strong	mStrong;
	Weak	mWeak;
	Partial mPartial;

	auto operator<=>(const StrongWeakPartial&) const = default;
};


// example Binary Coded Digit = BCD; by <=> we dont need to write comparisions for each member value

class BCD
{
public:
	constexpr BCD(int v, int Significance) : mSignificance{ Significance }, mValue{Adjust(v)} {}

	constexpr operator int() const { return mValue; }
	
	// already constexpr 
	auto operator<=>(const BCD&) const = default;


private:
	int mSignificance;
	int mValue;

	static constexpr int Adjust(int v);

};

constexpr int BCD::Adjust(int v)
{
	if (v > 9 || v < 0) { return 0; }
	return v;
}