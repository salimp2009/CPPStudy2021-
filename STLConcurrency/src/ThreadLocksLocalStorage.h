#pragma once
#include "ConcurrencyUtility.hpp"


inline void DeadLockSolution()
{
	std::printf("\n---------------Dead Lock Solution---------------------\n");
	/* alternatives that will not cause a deadlock between threads*/
	struct CriticalData
	{
		std::mutex mut;
	};

	auto deadLock = [](CriticalData& aa, CriticalData& bb)
	{
		/* defer lock allows to lock mutex when needed*/
		//std::unique_lock<std::mutex> guard1(a.mut, std::defer_lock);
		fmt::print("Thread: {}, first mutex!\n", std::this_thread::get_id());
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		//std::unique_lock<std::mutex> guard2(b.mut, std::defer_lock);
		fmt::print("Thread: {}, second mutex!\n", std::this_thread::get_id());

		/* lock guard will lock either both or none !*/
		//std::lock(guard1, guard2);
		
		/* C++17 addition; it can lock multiple locks*/
		std::scoped_lock guard(aa.mut,bb.mut);
		/* critical section!!*/
		fmt::print("Thread: {}, got both mutex!\n", std::this_thread::get_id());

	};

	CriticalData c1;
	CriticalData c2;

	std::thread t1([&] {deadLock(c1, c2); });
	std::thread t2(deadLock, std::ref(c1), std::ref(c2));

	t1.join();
	t2.join();

}

inline void SharedLock_Example()
{
	std::printf("\n---------------Shared Lock_Example---------------------\n");
	/*writer-reader lock;
	  NOTE: Reader-writer locks do not solve the fundamental problem - threads competing for access to a critical
	  region, but they do help to minimise the bottleneck
	*/

	std::map<std::string, int> teleBook{ {"Dijkstra", 1972}, {"Scott", 1976}, {"Ritchie", 1983} };

	std::shared_timed_mutex teleBookMutex;

	auto addToTeleBook = [&](const std::string& name, int year)
	{
		std::lock_guard<std::shared_timed_mutex> writerlock(teleBookMutex);
		fmt::print("Starting Update: {}\n", name);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		teleBook[name] = year;
		fmt::print("Ending Update: {}\n", name);
	};

	auto printNumber = [&](const std::string& name)
	{
		std::shared_lock<std::shared_timed_mutex> readerLock(teleBookMutex);
		auto searchResult = teleBook.find(name.data());
		if (searchResult != teleBook.end())
		{
			fmt::print("Reader result for name: {0}: {1}\n", name, teleBook[name]);
		}
		else
		{
			fmt::print("Reader : name: {0} not found!!!\n", name);
		}
	};

	std::thread reader1([&]() {printNumber("Scott"); });
	std::thread reader2([&]() {printNumber("Ritchie"); });
	std::thread w1([&](){addToTeleBook("Scott",1968);});
	std::thread reader3([&]() {printNumber("Dijkstra"); });
	std::thread reader4([&]() {printNumber("Scott"); });
	std::thread w2([&]() {addToTeleBook("Bjarne", 1965); });
	std::thread reader5([&]() {printNumber("Scott"); });
	std::thread reader6([&]() {printNumber("Ritchie"); });
	std::thread reader7([&]() {printNumber("Scott"); });
	std::thread reader8([&]() {printNumber("Bjarne"); });

	reader1.join();
	reader2.join();
	reader3.join();
	reader4.join();
	reader5.join();
	reader6.join();
	reader7.join();
	reader8.join();
	w1.join();
	w2.join();

	for (const auto& [name, year] : teleBook)
	{
		fmt::print("name: {0}, year: {1}\n", name, year);
	}
}

/* constant expression can be safe initialized in multithread environments so as constexpr*/
class MyDouble
{
private:
	double val1;
	double val2;
public:
	constexpr MyDouble(double v1, double v2) noexcept : val1{ v1 }, val2{ v2 } {}
	[[nodiscard]] constexpr auto GetValues() const { return std::make_pair(val1, val2); }
	[[nodiscard]] constexpr auto GetSum() const { return val1+val2; }
};

inline void ThreadSafeInitialize()
{
	std::printf("\n--------Thread Safe Initialize With Constants--------\n");

	/* All of the below are considered thread safe initialization*/
	constexpr auto myval = 25.0;
	
	constexpr MyDouble mystatics(10.0, myval);
	constexpr auto mystaticValues = mystatics.GetValues();

	const auto [v1, v2] = mystaticValues;

}


inline void CallOnce_ThreadSafeInit()
{
	std::printf("\n-------- CallOnce & Thread Safe Init--------\n");

	/* can be called once with multiple function but only one of the functions only one time executed!*/
	std::once_flag onceFlag;

	auto doOnce = [&]()
	{
		std::call_once(onceFlag, []() {fmt::print("calling only once!\n"); });
	};

	auto doOnce2 = [&]()
	{
		std::call_once(onceFlag, []() {fmt::print("calling only once2!\n"); });
	};

	std::thread t1{ doOnce };
	std::thread t2{ doOnce };
	std::thread t3{ doOnce2 };
	std::thread t4{ doOnce2 };

	t1.join();
	t2.join();
	t3.join();
	t4.join();
}

inline void Singleton_ThreadSafe()
{
	std::printf("\n------Singleton ThreadSafe with CallOnce------\n");

	std::cout << "MySingleton::getInstance(): " << MySingleton::getInstance() << '\n';
	std::cout << "MySingleton::getInstance(): " << MySingleton::getInstance() << '\n';

	std::cout << "MySingletonRM::getInstance(): " <<&(MySingletonRM::getInstance()) << '\n';
	std::cout << "MySingletonRM::getInstance(): " <<&(MySingletonRM::getInstance()) << '\n';
}

thread_local std::string threadName = "Hello from";

inline void ThreadLocalStorage_Basics()
{
	std::printf("\n------Thread Local StorageBasics------\n");
	
	std::mutex coutMutex;
	
	auto first = []() noexcept  {threadName += " first"; };
	auto second = []() noexcept {threadName += " second"; };
	auto third= []() noexcept  {threadName += " third"; };

	auto addThreadLocal = [&](const std::string& message)
	{
		/* global static variable thread_local does not need to be captured!!*/
		threadName += message;
		
		/* each thread will call the function and add that local variable 
		   to the threadName which at this point has the message of each thread*/
		first();
		second();
		third();
		std::lock_guard lock{ coutMutex };
		std::cout << "threadName: " << threadName << ", &threadName: " << &threadName << '\n';
		
		//fmt::print("fmt thread message : {0}, the message address: {1}\n", threadName,(void*)&threadName);
	};

	std::thread t1{ addThreadLocal," t1" };
	std::thread t2{ addThreadLocal, " t2" };
	std::thread t3{ addThreadLocal, " t3" };
	std::thread t4{ addThreadLocal, " t4" };

	t1.join();
	t2.join();
	t3.join();
	t4.join();

}

inline void ThreadLocalStorage_Basics2()
{
	std::printf("\n------Thread Local Storage Basics 2------\n");

	myThreadData.printData("function Basic2() begin: ");

	myThreadData.gName = "thread t1 name";
	myThreadData.lName = "thread t1 name";
	myThreadData.tName = "thread t1 name";

	myThreadData.printData("function Basic2() later: ");


	auto foo = []()
	{
		myThreadData.printData("foo lambda (): begin:");
		myThreadData.gName = "thread t2 name";
		myThreadData.lName = "thread t2 name";
		myThreadData.tName = "thread t2 name";
		myThreadData.printData("foo lambda (): end:");
	};

	std::thread t1(foo);
	t1.join();

	myThreadData.printData("function Basic2() end: ");

}

