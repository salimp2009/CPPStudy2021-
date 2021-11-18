#pragma once
#include "Templpch.h"

#include "ConceptsDispatch.hpp"
#include "Concepts/ConceptsFunctionValidate.hpp"
#include "ConceptsOptionalimpl.hpp"
#include "ConceptsSubsumptionRules.hpp"
#include "Concepts/ConceptsImprovedErrosMsg.hpp"

inline void conceptBasedDispatch()
{
	std::printf("\n---conceptBasedDispatch---\n");

	std::array<char, 16> ackData;
	
	std::array<char, 100> payLoadData;

	Send(ackData);
	Send(payLoadData);
}


std::integral auto getIntegral(int val)
{
	return val;
}
inline void placeHolders_Concepts()
{
	std::printf("\n---place Holders Concepts--\n");

	std::vector vec1 = { 1,2,3,4,5 };
	for (const std::integral auto& elem : vec1)
	{
		fmt::print("{} ", elem);
	}
	std::printf("\n");

	std::integral auto b = true;
	fmt::print("b: {}\n", b);

	auto integ1 = getIntegral(10);
	fmt::print("integ1: {}\n", integ1);
}

inline void dataValidateCheck_Concepts()
{
	std::printf("\n---place Holders Concepts--\n");

	ComplexType compt1{};
	SimpleType simpt2{};

	static_assert(SupportsValidation<ComplexType>);
	static_assert(not SupportsValidation<SimpleType>);

	ValidatedCheck::Send(compt1);
	ValidatedCheck::Send(simpt2);
}

inline void optionalImplemt_Concepts()
{
	static_assert(not std::is_copy_constructible_v<optional<notCopyable>>);
	static_assert(std::is_copy_constructible_v<optional<int>>);

	optional<Not_TriviallyDestructible> op1{};
	optional<COMLike> op2{};
}


inline void SubSumptionRules_Concepts()
{
	std::printf("\n---SubSumption Rules--\n");

	int a = 1;
	int b = 2;

	add(a, b);
}

inline void PrintContainers_Concepts()
{
	std::printf("\n--- Print Containers--\n");
	
	std::vector<int> vec1 = { 3,2,4,1,8};
	PrintSort(vec1);

	std::list l1 = { 5,1,7,9};
	PrintSort(l1);
}

