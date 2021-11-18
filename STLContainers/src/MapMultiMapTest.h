#pragma once

#include "STLContpch.h"
#include "OverLoadVariadic.hpp"


inline void MapMultiMap_Test()
{
	fmt::print("\n---------------------Map-MultiMap Test------------------------------------\n\n");

	std::map<int, std::string, std::greater<int>> coll1 = { {51,"Salim"}, {68, "Sema"} };
	printMCont(coll1);

	std::cout << coll1[51] << '\n';
	 
	std::map<int, std::string, std::greater<int>>coll2(coll1.begin(), coll1.end());
	printMCont(coll2);
	coll2[68] = "Semos";

	std::cout << std::boolalpha;
	/* equality checks both size of map and the values match or not; and the ordering needs to match*/
	std::cout << (coll2 == coll1) << '\n';

	for (auto& elem : coll2)
	{
		if (elem.second == "Salim")
		{
			coll2.erase(elem.first);
			/*if you dont use break then program will try iterate the deleted iterator and give runtime error*/
			break;
		}
	}

	printMCont(coll2);

	/*Alternative way to above code; iterate only if the value does not match*/
	for (auto pos = coll2.begin(); pos != coll2.end();)
	{
		if (pos->second == "Semos")
		{
			/** erase return the next pos so when we delete the current iterator program will increment over the next valid iterator
				or use pos++ inside the erase
			*/
			pos=coll2.erase(pos);
		}
		else
		{
			++pos;
		}
	}
	printMCont(coll2);

	coll2.emplace(50, "Didem");
	printMCont(coll2);

	printMCont(coll1);

	std::for_each(coll1.begin(), coll1.end(), [](auto& elem) { elem.second += " Pamukcu"; });
	std::for_each(coll1.begin(), coll1.end(), [](typename decltype(coll1)::value_type& elem){ elem.second += ":Family"; });
	std::for_each(coll1.begin(), coll1.end(), [](typename decltype(coll1)::value_type& elem){ elem.second += ":Family"; });

	printMCont(coll1);


	/* removed the node with the given key per c++17*/
	auto nodetoReplace = coll1.extract(51);
	/* change the key of the extracted node; which is just a pointer*/
	nodetoReplace.key() = 52;
	/* access to underlying types second value and change the first letter ; only for map; for sets use value() member funct of nodes*/
	nodetoReplace.mapped()[0] = 'Z';
	/* move the node back to std::map; no copies made only pointer moves with the new key*/
	coll1.insert(std::move(nodetoReplace));
	printMCont(coll1);

	/*Alternative way to rechange the key*/
	/* this is slower even for inserting prefer insert() member function*/
	/* the value type has to have a default constructor to use this as well*/
	coll1[51] = coll1[52];
	coll1[51][0] = 'S';
	coll1.erase(52);

	printMCont(coll1);

	coll1.insert(std::make_pair( 16,"Demir" ));
	printMCont(coll1);

	auto posSearched =std::find_if(coll1.cbegin(), coll1.cend(), [](const auto& elem) {return elem.second == "Demir"; });
	if (posSearched != coll1.cend())
	{
		fmt::print("Found the value {} we were looking for!\n", posSearched->second);
	}
	else
	{
		fmt::print("it does not exist !!!!\n");
	}

	/* Tried to use variant to change the Compare runtime; mightr better to use Lambda*/
	std::variant<NormalComp, ReverseComp>CompOpt{ NormalComp() };
	auto CurrentComp = std::get<0>(CompOpt);
	
	std::map<int, std::string, decltype(CurrentComp)>coll3 = { {55, "Salitos"}, {65, "Semoski"} };
	printMCont(coll3);

	CompOpt = ReverseComp();
	auto mYReverseComp = std::get<1>(CompOpt);
	std::map<int, std::string, decltype(mYReverseComp)>coll4 = { {55, "Salitos"}, {65, "Semoski"} };
	printMCont(coll4);

	/* Using variadic Base class */
	auto New1Comp = Overloader{ NormalComp() };

	std::map<int, std::string, decltype(New1Comp)>coll5 = { {55, "Salitos"}, {65, "Semoski"} };
	printMCont(coll5);

	auto New2Comp = Overloader{ ReverseComp() };
	std::map<int, std::string, decltype(New2Comp)>coll6(coll5.begin(), coll5.end()); 
	printMCont(coll6);



}
