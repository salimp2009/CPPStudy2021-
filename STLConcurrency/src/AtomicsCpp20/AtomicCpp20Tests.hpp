#pragma once
#include "ConcurrencyPCH.h"

#include "AtomicCpp20.hpp"

inline void AtomicRef()
{
	std::printf("\n-- AtomicRef--\n");

	ExpensiveToCopy2 expCopy{};

	count(expCopy);

	fmt::print("counter: {} \n", expCopy.counter);

	Counters count{ 4,5 };

	std::atomic_ref<Counters> atmCounters{ count };

	fmt::print("Before value change: atmCounters.a: {}, b: {} \n", atmCounters.load().a, atmCounters.load().b);

	count.a = 6;
	count.b = 7;

	fmt::print("After value change: atmCounters.a: {}, b: {} \n", atmCounters.load().a, atmCounters.load().b);

	fmt::print("Counters is_lock_free(): {}", atmCounters.is_lock_free());

}

inline void AtomicSharedPointer()
{
	std::printf("\n--AtomicSharedPointer--\n");

	std::atomic<std::shared_ptr<std::string>> sharedString(std::make_shared<std::string>("Zero"));

	std::thread t1([&sharedString]() 
	{
			// that is a very very expensive copy !!!
			sharedString.store(std::make_shared<std::string>(*sharedString.load() + "One"));
	});

	std::thread t2([&sharedString]()
		{
			// that is a very very expensive copy !!!
			sharedString.store(std::make_shared<std::string>(*sharedString.load() + "Two"));
		});
	
	std::thread t3([&sharedString]()
		{
			// that is a very very expensive copy !!!
			sharedString.store(std::make_shared<std::string>(*sharedString.load() + "Three"));
		});

	t1.join();
	t2.join();
	t3.join();

	fmt::print("sharedString: {} \n", *sharedString.load());
}

inline void AtomicFlag()
{
	std::printf("\n-AtomicFlag--\n");

	std::vector<int> myVec{};

// ATOMIC_FLAG_INIT is depreciated in C++20 and default constructor initializes since C++20 and it is constexpr
// https://en.cppreference.com/w/cpp/atomic/atomic_flag/atomic_flag
	std::atomic_flag atomicFlag{};

	auto prepareWork = [&myVec, &atomicFlag]()
	{
		myVec.insert(myVec.end(), { 0,1,0,3 });
		std::puts("Sender:Data prepared");
		atomicFlag.test_and_set(std::memory_order_release);
		atomicFlag.notify_one();
	};

	auto completeWork = [&myVec, &atomicFlag]()
	{
		std::puts("Waiter: waiting for work!!");
		atomicFlag.wait(false, std::memory_order_acquire);
		myVec[2] = 200;
		std::puts("Waiter:Completed Work!!");
		fmt::print("myVec: {}\n", myVec);
	};

	std::jthread t1(prepareWork);
	std::jthread t2(completeWork);
}