#pragma once
#include "RangesPCH.hpp"

template<std::ranges::input_range Range>
requires std::ranges::view<Range>
class ContainerView :public std::ranges::view_interface<ContainerView<Range>>
{
private:
	Range mRange{};
	std::ranges::iterator_t<Range> mbegin{ std::begin(mRange) };
	std::ranges::iterator_t<Range> mend{std::end(mRange)};

public:
	// this is needed to satisfy the view concept; https://en.cppreference.com/w/cpp/ranges/view_interface
	ContainerView() = default;
	
	constexpr ContainerView(Range r):mRange{std::move(r)}, mbegin{std::begin(r)}, mend{std::end(r)} { }

	constexpr auto begin() const { return mbegin; }
	constexpr auto end()   const { return mend; }
};

// Deduction Guide to make it easier for user
template<typename Range>
ContainerView(Range&& range)->ContainerView<std::ranges::views::all_t<Range>>;


inline void ContainerView_CustomView()
{
	std::printf("\n---ContainerView_CustomView---\n");

	std::vector<int> vec1 = { 1,2,3,4,5,6,7,8,9 };
	auto myContView = ContainerView(vec1);
	puts("User defined Container View: \n");
	for (auto elem : myContView)
	{
		std::printf("%d ", elem);
	}
	puts("\n");

	puts("Container View Reverse: \n");
	for (auto elem : std::views::reverse(myContView))
	{
		std::printf("%d ", elem);
	}
	puts("\n");

	puts("Container View first 4 elements: \n");
	for (auto elem : std::views::take(myContView, 4))
	{
		std::printf("%d ", elem);
	}
	puts("\n");

	std::string myStr = "Only for test purposes";

	auto myContainer2 = ContainerView(myStr);

	for (auto c : myContainer2)
	{
		fmt::print("{} ", c);
	}
	puts("\n");

	for(auto c: std::views::reverse(ContainerView(myStr)))
	{
		fmt::print("{} ", c);
	}
	puts("\n");

	for (auto c : ContainerView(myStr) |std::views::split(' '))
	{
		fmt::print("{} ", c);
	}
	puts("\n");

	auto print = [](auto const& view) {
		for (std::cout << "{ "; const auto element : view)
			std::cout << element << ' ';
		std::cout << "} ";
	};

	// example for cppreference ; print function print each splitted around {}; whereas the previos character prints each character in each splitted case 
	constexpr std::string_view hello{ "Hello C++ 20 !" };
	std::cout << "\n" "substrings: ";
	std::ranges::for_each(hello | std::ranges::views::split(' '), print);
}