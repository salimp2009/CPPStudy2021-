#pragma once
#include "ConceptUtilities.hpp"
#include "ConceptUtils02.hpp"
#include "Concepts/ConstraintAddAll.hpp"
#include "Concepts/ConceptsAddAllVers02.hpp"
#include "ConceptTestClass.hpp"



inline void greatCommonDiv_Concepts()
{
	std::printf("\n---great Common Div Concepts---\n");

	fmt::print("greaterCommonDivisor(100, 10): {}\n", greaterCommonDivisor(100, 10));
	fmt::print("greaterCommonDivisor1(5, 1): {}\n", greaterCommonDivisor1(5, 1));
	fmt::print("greaterCommonDivisor2(5, 1): {}\n", greaterCommonDivisor2(5, 1));
	fmt::print("greaterCommonDivisor3(5, 1): {}\n", greaterCommonDivisor3(5, 1));
	fmt::print("greaterCommonDivisor3(1, 5): {}\n", greaterCommonDivisor3(1, 5));
	fmt::print("greaterCommonDivisor4(1, 5): {}\n", greaterCommonDivisor4(1, 5));

	std::printf("addValues after concepts: %d \n", addAllValues(1, 2, 3, 4, 5, 6));
	std::printf("sum<unsigned i>(): %d \n", sum<5>(10));
}

struct Test 
{ 
	friend constexpr std::strong_ordering operator<=>(const Test& rhs, const Test& lhs) = default;  
};

struct Test1 { };

/*The regular concept specifies that a type is regular, that is, it is copyable, default constructible, and equality comparable. */
template<std::regular T>
class MyVector {};

struct NotCopyable
{
	NotCopyable() = default;
	NotCopyable(const NotCopyable&) = delete;
};

template<typename T>
struct MyVector2
{
	void push_back(const T&) requires std::copyable<T>{}
};

inline void compileTimePredicate_Concepts()
{
	std::printf("\n---compileTimePredicate with Concepts---\n");

	/* Use case of concepts in conditional checks!*/

	fmt::print("std::three_way_comparable<Test>: {}\n", std::three_way_comparable<Test>);
	fmt::print("std::three_way_comparable<Test1>: {}\n", std::three_way_comparable<Test1>);
	
	fmt::print("std::three_way_comparable<int>: {}\n", std::three_way_comparable<int>);
	fmt::print("std::three_way_comparable<double>: {}\n", std::three_way_comparable<double>);

	static_assert(std::three_way_comparable<std::string>);

	if constexpr (std::three_way_comparable<Test1>)
	{
		fmt::print("std::three_way_comparable<Test1>: True!\n");
	}
	else
	{
		fmt::print("std::three_way_comparable<Test1>: False!\n");
	}

	if constexpr (std::three_way_comparable<std::vector<int>>)
	{
		fmt::print("std::three_way_comparable<std::vector<int>>: True!\n");
	}
	else
	{
		fmt::print("std::three_way_comparable<std::vector<int>>: False!\n");
	}

	[[maybe_unused]] MyVector<int> myVec1;
	//[[maybe_unused]] MyVector<int&> myVec2;   // does not compile since it does not meet requirements of std::regular concept

	MyVector2<int> myvec22;
	myvec22.push_back(200);

	[[maybe_unused]] MyVector2<NotCopyable> myvec23;
	//myvec23.push_back(NotCopyable()); // does not compile since push_back() memb funct requires std::copyable<T>

}

inline void customConceptExample1_()
{
	std::printf("addValues2 with custom concepts: %d \n", addAllValues2(1, 2, 3, 4, 5, 6, 7));

	using RationalInt = Rational<int>;
	auto rationalSum = addAllValues2(RationalInt{ 3, 4 }, RationalInt{ 5, 8 }, RationalInt{ 5, 8 });

	std::printf("\nRational Numerator: %d, Denominator: %d \n", rationalSum.Numerator(), rationalSum.DeNominator());

	std::printf("\naddValues3 with custom concepts: %d \n", addAllValues3(1, 2, 3, 4, 5, 6, 7));
	auto rationalSum2 = addAllValues3(RationalInt{ 3, 4 }, RationalInt{ 5, 8 }, RationalInt{ 5, 8 });
	std::printf("Rational2 with AddAll3; Numerator: %d, Denominator: %d \n", rationalSum2.Numerator(), rationalSum2.DeNominator());

	std::printf("\naddValues4 with custom concepts: %d \n", AddAllValues4(1, 2, 3, 4, 5, 6, 7));
	auto rationalSum3 = AddAllValues4(RationalInt{ 3, 4 }, RationalInt{ 5, 8 }, RationalInt{ 5, 8 });
	std::printf("Rational3 with AddAll4; Numerator: %d, Denominator: %d \n", rationalSum3.Numerator(), rationalSum3.DeNominator());

	std::printf("\naddValues5 with custom concepts: %d \n", AddAllValues5(1, 2, 3, 4, 5, 6, 7));
	auto rationalSum4 = AddAllValues4(RationalInt{ 3, 4 }, RationalInt{ 5, 8 }, RationalInt{ 5, 8 });
	std::printf("Rational4 with AddAll5; Numerator: %d, Denominator: %d \n", rationalSum4.Numerator(), rationalSum4.DeNominator());

}

inline void ConceptsIn_VariadicTemplates()
{
	std::printf("\n---Concepts InVariadic Templates---\n");

	fmt::print("allValid(5, true, false): {}\n", allValid(5, true, false));
	fmt::print("anyValid(5, true, false): {}\n", anyValid(5, true, false));
	fmt::print("noneValid(5, true, false): {}\n", noneValid(5, true, false));
}


inline void TestingCustomConcept()
{
	/* Creating test objects with differen conditions to test concept!*/
	using NoAdd = ObjectStub<true, false, true, true>;
	using ValidClass = ObjectStub<true, true, true, true>;
	using NotNoEcept = ObjectStub<false, true, true, true>;
	using WithoutType = ObjectStub<true, true, false, true>;
	using DifferentReturnType = ObjectStub<true, true, false, false>;

	std::printf("\n---Testing Custom Concept---\n");

	/* assert + operator*/
	static_assert(Testclass<int, int>);
	static_assert(not Testclass<NoAdd, NoAdd>);
	
	/* assert no mix types allowed*/
	static_assert(not Testclass<int, float>);
	
	/* assert paramater has one type*/
	static_assert(not Testclass<int>);
	
	/* assert + operator is noexcept*/
	static_assert(not Testclass<NotNoEcept, NotNoEcept>);

	/* assert + operator return same type*/
	static_assert(not Testclass<DifferentReturnType, DifferentReturnType>);

	/* assert valid class works*/
	static_assert(Testclass<ValidClass, ValidClass>);

	/* assert there is a type in the class type*/
	/* if it is a class then it has to have type to pass the test
		if it is a built-in type like int it will still pass; tested above lines
	*/
	static_assert(not Testclass<WithoutType, WithoutType>);

}


inline void FunctionOverloadingConcepts()
{
	std::printf("\n---Function OverloadingConcepts---\n");

	/* Intellisense show false error !!!1*/
	std::list Li{ 1,2,3 };
	auto ItBi{ Li.begin() };
	advanceIt(ItBi, 1);

	std::forward_list fList = { 0,1,2,3 };
	auto fwIt = fList.begin();
	advanceIt(fwIt, 2);

	std::vector vec1{ 1,2,3,4 };
	auto RandIt = vec1.begin();
	advanceIt(RandIt, 2);
}

inline void SpecializeTemplates_Concepts()
{
	std::printf("\n---TemplateSpecializeConcepts---\n");
	DVector<int> dvec1;
	DVector<int&> dvec2;
}

inline void overloadWithConcepts()
{
	std::printf("\n---Overload With Concepts---\n");
	overload(3.14);
	overload(2021);
	overload(2023L);
}



