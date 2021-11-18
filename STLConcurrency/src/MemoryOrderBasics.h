#pragma once

#include "ConcurrencyUtility.hpp"



inline void AtomicFlag_Basics()
{
	std::printf("\n---------------Atomic Flag------------------------\n");

	/* by default std::atomic_flag is guaranteed to be lock free ; 
	other alternative std::atomic are implementation defined
	*/
	/* the default constructor or below macro (needed in C++11) initializes to false !!!*/
	std::atomic_flag readyFlag= ATOMIC_FLAG_INIT; 

#if _HAS_CXX20
	std::cout<<readyFlag.test()<<'\n';
#endif


}

/* modified with memory order/syncing using aqcquire-release */
class spinlock
{
private:
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
	void lock() noexcept
	{
		/* spinlock should be used for very short time locking ; it wastes CPU cycles; prefer not to use it*/
		while (flag.test_and_set(std::memory_order_acquire));
	}

	void unlock() noexcept
	{
		flag.clear(std::memory_order_release);
	}

};


inline void SpinLock_Example()
{
	std::printf("\n---------------Spinlock Example-----------------------\n");

	spinlock spin;

	const auto workOnResource = [&spin](auto&& val) noexcept
	{
		spin.lock();
		fmt::print("working on resource : {}, by thread : {}\n", ++val, std::this_thread::get_id());
		spin.unlock();
	};

	int myval{ 20 };

	std::thread t1(workOnResource, std::ref(myval));
	std::thread t2(workOnResource, std::ref(myval));

	t1.join();
	t2.join();

}

#if _HAS_CXX20
	inline void AtomicFlag_ThreadSync()
	{
		std::printf("\n---------------Atomic Flag _ Thread Syncing Example-----------------------\n");

		std::vector<int> myVec;
		std::atomic_flag atomicFlag{};

		const auto prepareWork = [&myVec, &atomicFlag]() noexcept
		{
			myVec.insert(myVec.end(), { 0,1,0,3 });
			std::printf("sender: data is ready !\n");
			atomicFlag.test_and_set();
			/* C++20 addition*/
			atomicFlag.notify_one();
		};

		const auto completeWork = [&myVec, &atomicFlag]() noexcept
		{
			/* wait while it is false*/
			/* Notes from cppreference.com: "..more efficient than simple polling or pure spinlocks.
			   Due to the ABA problem, transient changes from old to another value and back to old might be missed, and not unblock."
			*/
			/* C++20 addition .wait() function*/
			atomicFlag.wait(false);
			myVec[2] = 2;
			//fmt::print("completed work: myvec : {}", fmt::join(myVec, ", "));
			std::printf("completed work!\n");
			for (auto elem : myVec)
			{
				std::printf("%i%s", elem, (elem != myVec.back() ? "," : "\n"));
			}
		};

		std::thread t1{ prepareWork };
		std::thread t2{ completeWork };

		t1.join();
		t2.join();
}


/* a naive check if custom type can be atomic*/
template<typename T>
concept atomicTypes = !std::is_polymorphic_v<T>  &&  std::is_trivially_copy_constructible_v<T> &&  std::is_trivial_v<T>;

/* cpp versions of fethc_add or other operations returns the unchanged value; this custom version returns the new value
	+=, -= or other composite overloads return the new value and work on atomics
*/
template<atomicTypes T>
T fetch_Mult(std::atomic<T>& shared, T mult)
{
	T oldvalue = shared.load();
	while (!shared.compare_exchange_strong(oldvalue, oldvalue * mult));
	return oldvalue;
}

struct CustDef
{
	int a;
	int b;
};

inline void AtomicTypes_Operations()
{
	std::printf("\n---------------AtomicTypes &Operations-----------------------\n");

	int intArray[5];
	std::atomic<int*> ptr(intArray);

	ptr++;
	assert(ptr.load() == &intArray[1]);
	ptr++;
	assert(ptr.load() == &intArray[2]);
	ptr++;
	assert(ptr.load() == &intArray[3]);

	std::atomic<int> myInt{ 5 };
	fmt::print("myInt before change: {}\n", myInt);
	std::thread t1{fetch_Mult<int>, std::ref(myInt), 5 };
	std::thread t2{ fetch_Mult<int>, std::ref(myInt), 5 };
	t1.join();
	t2.join();

	fetch_Mult(myInt, 5);
	
	fmt::print("myInt after change: {}\n", myInt);

	/* fetch add returns the old value; it changes the value of the variable */
	std::printf("return value of fetch_add %i\n", myInt.fetch_add(5));

	std::atomic<CustDef> myCustDef;

	static_assert(std::is_trivially_copy_constructible_v<CustDef>);
	static_assert(atomicTypes<CustDef>);
	std::cout << !std::is_polymorphic_v<CustDef> && std::is_trivially_copy_constructible_v<CustDef> && std::is_trivial_v<CustDef> << '\n';

}



inline void SharedPtr_AtomicOps()
{
	std::printf("\n---------------SharedPtr & Atomic Operations-----------------------\n");

	//std::shared_ptr<int> ptr = std::make_shared<int>(2011);

	/* here we use shared ptr as a copy in the threads so it is OK but if a reference is to be used then 
		either use c++20 atmic shared ptr or use free function and a local shrptr then store a shared ptr atomicly; e.g. std::atomic_store(&ptr, localPtr)
		but only counter is thread safe not the underlying object!!!
	*/
	//for (auto i = 0; i < 10; ++i)
	//{
	//	std::thread{ [ptr, i]() 
	//		{
	//			std::shared_ptr<int> localPtr(ptr);
	//			localPtr = std::make_shared<int>(2014);
	//			fmt::print("i thread: {0}, ptr count: {1}, localPtr value:{2}\n", i, ptr.use_count(), *localPtr+i);
	//		} 
	//	
	//	}.detach();
	//}

	
	/* C++20 version of atomic; all atomic operation on other types now available to std::shared_ptr
		previous versions of std::atomic_store() and other free functions are depreceated and replace by new overloads
		C++11 versions will not work in C++20!!!
	*/
	std::atomic<std::shared_ptr<int>> shrPtr;
	shrPtr.store(std::make_shared<int>(2014));
	std::cout << *(shrPtr.load()) + 1 << '\n';

	std::thread t1{ [&shrPtr]() {std::cout << *(shrPtr.load()) + 5 << '\n'; } };

	std::cout << shrPtr.load().use_count()<< '\n';

	t1.join();

/* prefer not to use detach !!!
	
*/
	//for (auto i = 0; i < 10; ++i)
	//{
	//	std::thread{ [&shrPtr, i]() 
	//		{
	//			fmt::print("i thread: {0}, ptr count: {1}, localPtr value:{2}\n", i, shrPtr.load().use_count(), *(shrPtr.load())+i);
	//		} 
	//
	//	}.detach();
	//}

}

struct ExpensiveToCopy
{
	int counter{};
};

inline void AtomicRef_Example()
{
	std::printf("\n---------------Atomic Ref C++20 Example-----------------------\n");

	auto getRandom = [](int begin, int end) noexcept ->int
	{
		std::random_device seed;
		std::mt19937 engine(seed());
		std::uniform_int_distribution<> uniformDist(begin, end);
		return uniformDist(engine);
	};

	auto count = [&getRandom](ExpensiveToCopy& exp) noexcept
	{
		std::vector<std::thread> vthreads;
		/* the values of counter will not changed since each thread makes its own copy*/
		//std::atomic<int> counter{exp.counter};

		/*if you need to change the counter then use atomic_ref in c++20*/
		std::atomic_ref<int>counter{ exp.counter };

		for (auto n = 0; n < 10; ++n)
		{
			vthreads.emplace_back([&counter, &getRandom]()
				{
					auto randomNumber = getRandom(100, 200);
					for (auto i = 0; i < randomNumber; ++i) { ++counter; }
				});
		}

		for (auto& t : vthreads)
		{
			t.join();
		}
	
	};

	ExpensiveToCopy exp;
	count(exp);

	/* the value of exp.counter changes since we used atomic_ref within the lambda
		if you dont have c++20 then just pass the counter into lamda as an argument by ref and create the inner
		lambda before passing into thread and pass the lambda and its expected argument into thread by std::ref
	*/
	fmt::print("exp.counter expected to change : {}\n", exp.counter);
	
}

#endif

inline void AcquireRelease_Example()
{
	std::printf("\n--------------- Acquire Release Example-----------------------\n");

	std::vector<int> mySharedWork;
	std::atomic<bool> dataProduced{ false };
	std::atomic<bool>dataConsume{ false };

	auto produceData = [&mySharedWork, &dataProduced]() noexcept
	{
		mySharedWork = { 1,0,3 };
		fmt::print("produced data! \n");
		dataProduced.store(true, std::memory_order_release);
	};

	auto deliverData = [&dataProduced, &dataConsume]() noexcept
	{
		while (!dataProduced.load(std::memory_order_acquire));
		fmt::print("delivered data: current value of dataConsume: {0}, dataProduce: {1}\n", dataConsume, dataProduced);
		dataConsume.store(true, std::memory_order_release);
		
	};

	auto consumeData = [&dataConsume, &mySharedWork, &dataProduced]() noexcept
	{
		while (!dataConsume.load(std::memory_order_acquire));
		mySharedWork[1] = 55;
		fmt::print("consumed data: current value of dataConsume: {0}, dataProduce: {1}\n", dataConsume, dataProduced);
	};

	auto th1 = std::thread(consumeData);
	auto th2 = std::thread(deliverData);
	auto th3 = std::thread(produceData);

	th1.join();
	th2.join();
	th3.join();

	fmt::print("main thread: current value of dataConsume: {0}, dataProduce: {1}\n", dataConsume, dataProduced);
	printCont(mySharedWork);

}

inline void ReleaseSemantics_Example()
{
	std::printf("\n--------------- Release Semantics Example-----------------------\n");
	std::atomic<int> atomInt{ 0 };
	int somethngShared{ 0 };

	using namespace std::chrono_literals;

	auto writeShared = [&somethngShared, &atomInt]() noexcept
	{
		somethngShared = 2011;
		atomInt.store(2, std::memory_order_release);
	};

	auto readShared = [&somethngShared, &atomInt]() noexcept
	{
		while (!(atomInt.fetch_sub(1, std::memory_order_acquire) > 0))
		{
			std::this_thread::sleep_for(100ms);
		}

		fmt::print("somethng Shared: {}\n", somethngShared);
	};

	std::thread t1{ writeShared };
	std::thread t2{ readShared };
	std::thread t3{ readShared };

	t1.join();
	t2.join();
	t3.join();

	fmt::print("atomic Int: {}\n", atomInt);
}

inline void AcquireConsumeRelease_Example()
{
	std::printf("\n--------------- Acquire Consume Release----------------------\n");
	/* !!---DO NOT USE std::memory_order_consume since the C++ standart is revising it and NOT recommended---!!;
		“The specification of release-consume ordering is being revised,
		and the use of memory_order_consume is temporarily discouraged.”
	*/

	std::atomic<std::string*> ptr;
	int data{ 0 };
	std::atomic<int> atomData;

	auto producer = [&ptr, &data, &atomData]() noexcept
	{
		std::string* p = new std::string("C++11");
		data = 2011;
		atomData.store(2014, std::memory_order_relaxed);
		fmt::print("data produced!!\n");
		ptr.store(p, std::memory_order_release);
	};

	auto consumer = [&ptr, &data, &atomData]() noexcept
	{
		std::string* p2;
		
		/* std::memory_order_consume only carries a depency between threads on the atmoic variable that it is bound to
		   the other variable not related to same dependency can be accessed in different order but compilers under the hood use 
		   use same implementation with the acquire therefore it does not create a problem but DONT USE IT UNTIL c++ standart revises
		*/
		//while (!(p2 = ptr.load(std::memory_order_consume)));
		
		/* if p2 is nullptr wait*/
		while (!(p2 = ptr.load(std::memory_order_acquire)));
		fmt::print("*p2: {}\n", *p2);
		fmt::print("data: {}\n", data);
		fmt::print("atomData: {}\n", atomData.load(std::memory_order_relaxed));
	};

	std::thread t1{ consumer };
	std::thread t2{ producer };

	t1.join();
	t2.join();
}

inline void RelaxedSemantics_Example()
{
	std::printf("\n--------------- Relaxed Semantics Release----------------------\n");

	std::atomic<int> counter{ 0 };

	auto add = [&counter]() noexcept
	{
		for (auto n = 0; n < 1000; ++n)
		{
			counter.fetch_add(1, std::memory_order_relaxed);
		}
	};

	std::vector<std::thread>threads;

	for (auto i = 0; i < 10; ++i)
	{
		threads.emplace_back(add);
	}

	for (auto& t : threads)
	{
		t.join();
	}

	fmt::print("final counter value: {}\n", counter);
}

inline void FencesAcquireRelease_Example()
{
	std::printf("\n--------------- Fences Acquire Release---------------------\n");

	std::atomic<std::string*> ptr;
	int data{ 0 };
	std::atomic<int> atomData;

	auto producer = [&ptr, &data, &atomData]() noexcept
	{
		std::string* p = new std::string("C++11");
		data = 2011;
		atomData.store(2014, std::memory_order_relaxed);
		
		/* thread fence release protects the write after it and no load or write before can cross the fence*/
		/* atomic store operation below is protected by the fence*/
		std::atomic_thread_fence(std::memory_order_release);
		fmt::print("thread fence release: data produced!!\n");
		ptr.store(p, std::memory_order_relaxed);
	};

	auto consumer = [&ptr, &data, &atomData]() noexcept
	{
		std::string* p2;
		/* atomic load(read) operation is protected by the atmic fence acquire below it
			all other operations below can cross the fence
		*/
		while (!(p2 = ptr.load(std::memory_order_relaxed)));
		std::atomic_thread_fence(std::memory_order_acquire);
		fmt::print("*p2: {}\n", *p2);
		fmt::print("data: {}\n", data);
		fmt::print("atomData: {}\n", atomData.load(std::memory_order_relaxed));
	};

	std::thread t1{ consumer };
	std::thread t2{ producer };

	t1.join();
	t2.join();
}

/* Used in atomic signal fence example!!*/
std::atomic<bool> a{ false };
std::atomic<bool> b{ false };

extern "C" void handler(int)
{
	if (a.load(std::memory_order_relaxed))
	{
		std::atomic_signal_fence(std::memory_order_acquire);
		assert(b.load(std::memory_order_relaxed));
		
	}
}


inline void FencesAtomicSignal_Example()
{
	std::printf("\n---------------Atomic Signal Fence---------------------\n");


	/* std::atomic_signal_fence establishes memory synchronisation ordering of non-atomic and relaxed atomic
		accesses between a thread and a signal handler executed on the same thread
	*/

	/* SIGTERM ; signal termination request*/
	std::signal(SIGTERM, handler);

	b.store(true, std::memory_order_relaxed);
	std::atomic_thread_fence(std::memory_order_release);
	a.store(true, std::memory_order_relaxed);

	/*if memory order relaxed is used on these loads below the release fence 
	 the fence wont prevent them being reordered if relaxed is used since the release fence
	 protects the writes/store below and the load/store above 	
	*/
	fmt::print("atomic a: {0}, atomic b: {1}", a.load(), b.load());
}

