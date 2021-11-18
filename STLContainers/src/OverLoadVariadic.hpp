#pragma once


struct NormalComp
{
	template<typename T>
	bool constexpr operator()(T&& t1, T&& t2) const noexcept
	{
		return  t1 < t2 ;
	}
};

struct ReverseComp
{
	template<typename T>
	bool constexpr operator()(T&& t1, T&& t2) const noexcept
	{
		return  t2 < t1;
	}
};


template<class... Bases>
struct Overloader : Bases...
{
	using Bases::operator()...;
};

template<class... Bases>Overloader(Bases...) -> Overloader<Bases...>;


