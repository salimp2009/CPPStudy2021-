#pragma once
#include "ConcurrencyPCH.h"

struct ExpensiveToCopy2
{
	int counter{};
};


int getRandom(int begin, int end)
{
	std::random_device seed;			// seed
	std::mt19937 engine(seed());		// generator
	std::uniform_int_distribution<> uniformDist(begin, end);

	return uniformDist(engine);
}

void count(ExpensiveToCopy2& exp)
{
	std::vector<std::thread> v;
	//std::atomic<int> counter{ exp.counter };  // this copies ex.counter therfore we cannot access the value and not desirable if it is expensive to copy

	std::atomic_ref<int>counter{ exp.counter };

	for (int n = 0; n < 10; ++n)
	{
		v.emplace_back([&counter]() 
		{
				auto randomNumber = getRandom(100, 200);
				for (int i = 0; i < randomNumber; ++i)
				{
					++counter;
				}
		});
	}

	for (auto& thread : v)
	{
		thread.join();
	}

}

struct Counters
{
	int a;
	int b;
};