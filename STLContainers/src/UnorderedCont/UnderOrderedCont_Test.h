#pragma once

//#include "STLContpch.h"
#include "ContainerUtilities.hpp"
#include "CustomHashEqual.h"

inline void UnorderedCont_Test()
{
	fmt::print("\n---------------------Unordered Containers Test------------------------------------\n\n");

	std::unordered_map<std::string, float>mcoll1 = { {"Salim", 51.0f}, {"Didem", 50.0f}, {"Semos", 68.0f}, {"Demir", 16.0f} };
	mcoll1.max_load_factor(0.7f);
	fmt::print("load factor: {0}, bucket count: {1} \n", mcoll1.load_factor(), mcoll1.bucket_count());
	
	printMCont(mcoll1);

	static_assert(std::is_same_v<typename std::unordered_map<std::string, float>::value_type, std::pair<const std::string, float>>);

	/** Iterate over each bucket; buckets has their local iterator that goes thru
		Therefore you have first getinto bucket star from zero to bucket_count
		Once you are in a bucket you go over each elemet using iterator
	*/
	for (std::size_t idx = 0; idx != mcoll1.bucket_count(); ++idx)
	{
		std::cout << " b[" << std::setw(2) << idx << "]: ";
		for (auto pos = mcoll1.begin(idx); pos != mcoll1.end(idx); ++pos)
		{
			std::cout <<*pos<<" ";
		}
		std::cout << '\n';
	}
	printBCont(mcoll1);

	mcoll1["Salim"] = 52;
	printMCont(mcoll1);

	auto newpos=mcoll1.find("Didem");
	if (newpos != mcoll1.end()) newpos->second = 51;
	printMCont(mcoll1);

	std::unordered_set<int>scoll2 = { 1,-3,3,5, 5,-25,13,-14 };
	printCont(scoll2);
	printBCont(scoll2);

	scoll2.erase(5);
	printCont(scoll2);

	/* Combine Hash and Equal using variadic bases to construct both; similar to std::variant*/
	/* both Equality function and Hashfunction has to const qualified to be able to pass in unordered_maps*/
	using CustomerOP1 = Overloader<CustomerHash, CustomerEqual>;
	std::unordered_set<Customer, CustomerOP1, CustomerOP1>scoll3{{"Salim", "Pamukcu", 5201}};
	printCont(scoll3);
	printBCont(scoll3);

	/* Test Overloader if it can generate lambda; might be usefull to use as function wrapper instead of*/
	int z = 25;
	auto funcoverload = Overloader([&](auto&& x)->decltype(auto){std::cout << "x and z " << x <<", "<<z <<'\n'; }, [](float y) {std::cout << "y: " << y<< '\n'; });
	funcoverload(10.235f);
	funcoverload(10);
		

	/* Lambda version of Hash; need to pass lambda as a variable in the constructor since lambda dont have constructor*/
	auto CustHashLamb = [](auto&& c1) constexpr noexcept { return  std::hash<std::string_view>{}(c1.GetFName()) ^ (std::hash<std::string_view>{}(c1.GetLName())<<1) + (std::hash<long>{}(c1.GetNo())>>2); };
	auto CustEqLamb = [](auto&& c1, auto&& c2) constexpr noexcept { return c1 == c2; };
	std::unordered_set<Customer, decltype(CustHashLamb), decltype(CustEqLamb)>scoll4{10,CustHashLamb, CustEqLamb};
	
	scoll4.max_load_factor(0.7f);
	scoll4.insert({ {"Salim", "Pamukcu", 5201},{"Didem", "Pamukcu", 5200} });
	printCont(scoll4);
	printBCont(scoll4);
	
	std::cout <<"bucket count: "<< scoll4.bucket_count() << '\n';

	std::cout << std::boolalpha;
	auto SinglyLinkedList = !std::is_same_v<typename std::iterator_traits<decltype(scoll3)::iterator>::iterator_category, std::bidirectional_iterator_tag>;
	std::cout << "Chaining Type; Singly Linked List ????: " << SinglyLinkedList << '\n';
}