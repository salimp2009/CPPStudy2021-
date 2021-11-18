#pragma once
#include "RangesPCH.hpp"

template<typename Func, std::ranges::input_range Seq>
inline constexpr auto filter(Func func, const Seq& seq)
{
	using value_type = typename Seq::value_type;

	std::vector<value_type> result{};
	

	for (auto i : seq | std::views::filter(func))
	{
		result.emplace_back(i);
	}

	return result;
}

void FilterRanges_PythonStyle();

template<typename Func, typename Seq>
inline constexpr auto map(Func func, const Seq& seq)
{
	using value_type = typename Seq::value_type;
	using return_type = decltype(func(std::declval<value_type>()));

	std::vector<return_type> result{};

	for (auto i : seq | std::views::transform(func))
	{
		result.push_back(i);
	}
	return result;
}

void TransforMapRanges_PythonStyle();

template<typename T>
struct AlwaysTrue
{
	constexpr bool operator()(const T&) const
	{
		return true;
	}
};

template<typename Map, typename Seq, typename Filt = AlwaysTrue<typename Seq::value_type>>
auto MapFilter(Map map, Seq seq, Filt filt = Filt())
{
	using value_type = typename Seq::value_type;
	using return_type = decltype(map(std::declval<value_type>()));

	std::vector<return_type> result{};

	for (auto i : seq | std::views::filter(filt) | std::views::transform(map))
	{
		result.push_back(i);
	}

	return result;
}

void MapFilterRanges_PythonStyle();