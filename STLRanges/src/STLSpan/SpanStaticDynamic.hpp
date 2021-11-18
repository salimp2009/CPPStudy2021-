#pragma once
#include "RangesPCH.hpp"

inline void DynamicExtent_Span()
{
	std::printf("\n---DynamicExtent_Span---\n");

	auto printCont = [](std::span<int> container)
	{
		fmt::print("container size : {}\n", container.size());
		fmt::print("{}\n", fmt::join(container, ", "));
	};

	int arr[] = { 1,2,3,4,5 };
	printCont(arr);

	std::vector vec1{ 1,2,4,5 };
	printCont(vec1);

	std::array arr2{ 1,2,3, };
	printCont(arr2);

}

inline void StaticDynamicExtent_Span()
{
	std::printf("\n---StaticDynamicExtent_Span---\n");
	auto printCont = [](std::span<int> container)
	{
		fmt::print("container size : {}\n", container.size());
		fmt::print("{}\n", fmt::join(container, ", "));
	};

	std::vector<int> myVec1{ 1,2,3,4,5 };
	std::vector myVec2{ 1,2,3,4 };

	std::span<int>	  dynamicSpan{ myVec1 };
	std::span<int, 4> staticSpan{ myVec2 };

	std::span pointerSpan{ myVec2.data(), myVec2.size() };

	printCont(dynamicSpan);
	printCont(staticSpan);
	printCont(pointerSpan);

	fmt::print("size of dynamic span : {}\n", sizeof(dynamicSpan));
	fmt::print("size of static span : {}\n", sizeof(staticSpan));
	fmt::print("size of pointer span : {}\n", sizeof(pointerSpan));

	auto spansEqual = std::equal(staticSpan.begin(), staticSpan.end(), pointerSpan.begin(), pointerSpan.end());
	fmt::print("spansEqual: {}\n", spansEqual);

	std::string myString1 = { "heeeeeee\n" };
	std::span stringSpan{ myString1.data(),  myString1.data()+1 };
}


inline void spanTransform_ModifyElem()
{
	std::printf("\n---spanTransform_ModifyElem---\n");
	auto printCont = [](std::span<int> container) 
	{
		fmt::print("container size : {}\n", container.size());
		fmt::print("{}\n", fmt::join(container, ", "));
	};

	std::vector myVec1 = { 1,2,3,4,5,6,7,8,9,10 };

	std::span span1{ myVec1 };
	std::span span2{ span1.subspan(2 ,span1.size()-4) };
	printCont(span2);


	// changes the original vectors element thru subspan span2; only the elements subspan refers are changed in the original container
	std::ranges::transform(span2, span2.begin(), [](auto&& elem) { return elem * elem; });
	
	// ranges::transform applied to func to all elements but return a range, view ; does not change the original container
	auto res = span2 | std::views::transform([](auto&& elem) { return elem * elem; });
	
	printCont(myVec1);
	printCont(span1);
	printCont(span2);
	for (auto elem : res)
	{
		fmt::print("{} ", elem);
	}
	puts(" ");
}


inline void SpanMemberFunctions()
{
	std::printf("\n---SpanMemberFunctions--\n");
	auto printCont = [](std::span<int> container)
	{
		fmt::print("container size : {}\n", container.size());
		fmt::print("{}\n", fmt::join(container, ", "));
	};
	std::vector<int>myVec(20);
	std::iota(myVec.begin(), myVec.end(), 0);
	printCont(myVec);

	std::span mySpan{ myVec };
	auto length = mySpan.size();

	std::size_t increment = 5;

	for (std::size_t first = 0; first < length-increment ; first += increment)
	{
		for (auto elem : mySpan.subspan(first, increment))
		{
			fmt::print("{} ", elem);
		}
		std::putchar('\n');
	}
	
}

inline void ConstRange_ModifiableObjects()
{
	std::printf("\n---ConstRange_ModifiableObjects--\n");

	auto printCont = [](std::span<int> container)
	{
		fmt::print("container size : {}\n", container.size());
		fmt::print("{}\n", fmt::join(container, ", "));
	};

	auto printConstCont = [](std::span<const int> container)
	{
		fmt::print("container size : {}\n", container.size());
		fmt::print("{}\n", fmt::join(container, ", "));
	};

	std::vector<int> origVec1{ 1,2,3,4,5,6 };

	// variable range
	std::vector dynamicRange = origVec1;
	// although the the vector is changed original source is still same 
	// but if you std::span then any change changes the original as well
	dynamicRange[2] = 55;
	printCont(dynamicRange);

	//constant range of constant elements;
	const std::vector constRange = origVec1;
	//constRange[2] = 3;    // will not compile

	// const range of constant elements
	std::span<const int> constSpan{ origVec1 };
	//constSpan[2] = 33;   // will not compile!!
	printConstCont(constSpan);

	// span can be assigned to another constant range
	constSpan = dynamicRange;
	printConstCont(constSpan);

	// constant range with modifyable elements
	std::span<int> dynamicSpan{ origVec1 };
	dynamicSpan[2] = 55;
	printConstCont(origVec1);
	printCont(origVec1);
	printConstCont(dynamicSpan);

	// if the original vector adds an element the iterators will be changed therefore
	// previously defined std::span will refer to previous iterator therefore it will be undefined behaviour
	origVec1.push_back(255);
	printCont(origVec1);

	// iterators are changed but dynamicSpan refers to old iterator so Undefined Behaviour !!!
	//printCont(dynamicSpan);
	
	dynamicSpan = origVec1;
	printCont(dynamicSpan);
}
