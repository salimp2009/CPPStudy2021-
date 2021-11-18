#pragma once
#include "RangesPCH.hpp"

inline void FilterView_LazyEvaluation()
{
	std::printf("\n-FilterView_LazyEvaluation-\n");

	std::vector<int>coll{ 8,15,7,0,9,12};

	// this defines the view but apply all these functions
	auto vColl = coll | std::views::filter([](auto i)
						{
							std::printf("filter: %d\n", i);
							return i % 3 == 0;
						})
					  | std::views::transform([](auto i)
						{
							  std::printf("transform: %d\n", i);
							  return i = -i;
						});

	// when we call the elements of the defined view it start applying function and print a value
	// this is called Lazy Evaluation
	for (auto elem : vColl)
	{
		std::printf(" => %d\n", elem);
	}
}

inline void FilterView_PerformaceIssue()
{
	std::printf("\n-FilterView_PerformaceIssue-\n");

		std::vector<int>coll{ 8,15,7,0,9,12};

	// Using filter at the end cause transform to be called twice when the filter cond is returned true
	// this only happens with views::filter ; so rule of thum use filter at the beginning of piping
	// try to use pass by value into view or const& othwerwise might cause undefined behaviour except views::drop which can be passed by & 
	auto vColl = coll | std::views::transform([](auto i)
						{
						std::printf("transform: %d\n", i);
						return i = -i;
						})
						
					   | std::views::filter([](auto i)
					   	{
					   		std::printf("filter: %d\n", i);
					   		return i % 3 == 0;
					   	});
					 

	// when we call the elements of the defined view it start applying function and print a value
	// this is called Lazy Evaluation
	for (auto elem : vColl)
	{
		std::printf(" => %d\n", elem);
	}

	// typicall it is better to use by value but in this case we assign zero to 
	// allelements other than first 3
	for (auto& elem : coll | std::views::drop(3))
	{
		elem = 0;
		fmt::print("elem new value: {}\n",elem);
	}

	fmt::print("{}", coll);
	puts("\n");

	//views::take_while when combined / piped with other views it creates unncessary calls; try not to use with other views
	for (auto x : std::views::iota(0, 20)
		| std::views::take_while([](auto value)
			{
				fmt::print("take_While: {}\n", value);
				return value * 2 < 10;
			})
		| std::views::transform([](auto value)
			{
				fmt::print("transform: {}\n", value);
				return value / 2;
			})
				)
	{
		fmt::print("value: {}\n", x);
	}
	
	// views::take_while better used like this without piping other views as explained above !
	for (int year : std::views::iota(2017)
		| std::views::take_while([](int y) { return y <= 2020; })) {
		fmt::print("year: {} ", year);
	}
	puts("\n");
}
