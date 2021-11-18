#pragma once
#include "TemplateStack.hpp"
#include "StringStackSpecialize.hpp"
#include "Stack2VectorCont.hpp"


inline void StackExamples()
{
	std::printf("\n---------------------Stack Test- LIFO(Last in First Out)------------------------------------\n\n");

	std::stack<int> st1;

	st1.push(1);
	st1.push(2);
	st1.push(3);

	fmt::print("top: {}\n", st1.top());
	st1.pop();
	fmt::print("top: {}\n", st1.top());
	st1.top() = 255;
	fmt::print("top: {}\n", st1.top());

	int val{ 0 };
	if (!st1.empty())
	{
		val = st1.top();
		st1.pop();
	}
	fmt::print("top: {}\n", st1.top());
	fmt::print("val (old st1 top): {}\n", val);

	SPStack<int>mstack{ 1,2,3,4,5,70,8,9,10 };

	for (auto elem : mstack)
	{
		std::cout << elem << '\n';
	}

	//mstack.push(1);
	fmt::print("mstack top: {}\n", mstack.top());

	auto stackval = mstack.emplace(6, 7, 8);
	fmt::print("mstack top: {}\n", mstack.top());
	fmt::print("mstack top: {}\n", stackval);

	SPStack<int>mstack2{ mstack };
	mstack2.emplace(55);
	fmt::print("mstack top: {}\n", mstack2.top());

	SPStack<std::string>mstack3{ "Salim", "Didem", "Demir" };
	fmt::print("mstack top: {}\n", mstack3.top());
	fmt::print("SPStack empty() function test: {0}\n", mstack3.empty());

	SPStack mstackTest = { "Salitos" };
	mstackTest = mstack3;
	for (const auto& elem : mstackTest)
	{
		fmt::print("testing spstackTest: {}\n", elem);
	}

	auto removedval = mstack3.pop();
	fmt::print("mstack top: {}\n", mstack3.top());
	fmt::print("mstack top: {}\n", removedval);
	mstack3.push("Semos");
	fmt::print("mstack top: {}\n", mstack3.top());

	for (auto elem : mstack3)
	{
		fmt::print("{0}{1} ", elem, (elem==mstack3.top() ? " ":", "));
	}

	SPStack<float*> mstack4;
	mstack4.emplace(new float(5));
	fmt::print("mstack top: {}\n", *mstack4.top());
	auto fptr = mstack4.pop();
	fmt::print("mstack top: {}\n", *fptr);
	delete fptr;

	SPStack<SPStack<int>>mstack5{ 1,2,3 };

	fmt::print("mstack top: {}\n", mstack5.top().top());
	

	SPStack<std::pair<int, float>>mstack6;
	mstack6.push({ 4,45.0f });
	fmt::print("mstack with pair top: {}\n", mstack6.top().second);
	mstack6.emplace(std::make_pair(12, 12.5f), std::make_pair(12, 35.5F));
	mstack6.emplace({ 12, 34.4f });

	std::vector<std::pair<int, float>> vec1{ { 4,45.0f } };
	vec1.emplace_back(12, 12.5f);

	SPStack2<std::pair<int, float>>vecstck1;
	vecstck1.emplace({ 12, 34.4f });
	vecstck1.push({ 4, 45.f });
	fmt::print("vector stack top: {}\n", vecstck1.top().second);

	SPStack2 strstck2{"salim"};
	strstck2.push("Didem");
	fmt::print("string stack top: {}\n", strstck2.top());

	SPStack stringStack="Demir";
	SPStack2 stringStack2{ "Salitos" };
	SPStack2 intStack = {1};


}

inline void Queues_Examples()
{
	std::printf("\n---------------------Queues Test- FIFO(First-in-First-Out)------------------------------------\n\n");

	std::queue<std::string>q1;
	q1.push("Salim");
	fmt::print("queue front: {}\n", q1.front());
	q1.push("Didem");
	q1.push("Demir");
	q1.push("Semos");
	fmt::print("queue front: {}\n", q1.front());
	fmt::print("queue front: {}\n", q1.back());
	q1.pop();
	fmt::print("queue front: {}\n", q1.front());
}

inline void PriorityQueue_Examples()
{
	std::printf("\n---------------------Priority Queue Test- FIFO(First-in-First-Out)------------------------------------\n\n");

	std::priority_queue<float>pq1;

	pq1.push(120.5f);
	pq1.push(20.5f);
	pq1.push(12.5f);
	pq1.push(180.5f);
	while (!pq1.empty())
	{
		fmt::print("pq1 top : {}\n", pq1.top());
		pq1.pop();
	}

	
}