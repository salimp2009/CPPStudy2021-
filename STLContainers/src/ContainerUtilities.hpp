#pragma once
//#include "STLContpch.h"



template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p)
{
	return os << "[" << p.first << ", " << p.second << "] ";
}


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& cont) 
{
	for (const auto& elem : cont)
	{
		os << elem << " ";
	}
	return os;
}


template<typename Cont>
constexpr void printCont(Cont& cont)
{
	if (cont.empty()) return;

	for (auto&& elem : cont)
	{
		fmt::print("{} ", elem);
	}
	std::cout << '\n';
}

template<typename T1, typename T2>
constexpr void printCont(std::map<T1, T2>& cont)
{
	if (cont.empty()) return;

	for (auto&& [key, value]: cont)
	{
		fmt::print("[key: {0}, value: {1}] ", key, value);
	}
	std::cout << '\n';
}



template<typename MCont>
constexpr void printMCont(MCont& cont)
{
	if (cont.empty()) return;

	for (auto&& [key, value] : cont)
	{
		fmt::print("[key: {0}, value: {1}] ", key, value);
	}
	std::cout << '\n';
}


template<typename MCont>
constexpr void printBCont(MCont& cont)
{
	for (std::size_t idx = 0; idx != cont.bucket_count(); ++idx)
	{
		fmt::print("b[{}]: ", idx);
		for (auto pos = cont.begin(idx); pos != cont.end(idx); ++pos)
		{
			std::cout << *pos << " ";
		}
		std::cout << '\n';
	}

}


template<typename InputIterator, typename Tval>
constexpr InputIterator find_before(InputIterator first, InputIterator last, Tval&& val)
{
	if (first == last)
	{
		return first;
	}

	InputIterator next{ first };
	++next;

	while (next != last && *next != val)
	{
		++next;
		++first;
	}

	return first;
}

template<class InputIterator, class Tpred>
constexpr InputIterator find_before_if(InputIterator first, const InputIterator last, Tpred&& pred)
{
	if (first == last)
	{
		return first;
	}

	InputIterator next{ first };
	++next;

	while (next != last && !pred(std::forward<decltype(*next)>(*next)))
	{
		++next;
		++first;
	}

	return first;
}



