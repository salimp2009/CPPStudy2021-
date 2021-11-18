#pragma once
#include "CoroutinesLazyGenerator.hpp"
#include "CoroutineFactoryBasics.hpp"
#include "CoroutinesEagerFuture.hpp"
#include "ByteStreamParserOLDWay.hpp"
#include "CortnInfiniteDataStream.hpp"
#include "CoroutinesLazyFutures.hpp"
#include "CoroutinesFutureOnThread.hpp"
#include "CoroutinesTransparentAwaiter.hpp"
#include "CoroutinesAutoResumeAwaiter.hpp"
#include "CoroutinesAwaiterThread.hpp"
#include "CoroutineBasics.hpp"
#include "CoroutineBasicsYield.hpp"
#include "CoroutineBasicsIterator.hpp"
#include "CoroutineBasicsCoReturn.hpp"



inline void LazyGenerator_Coroutine()
{
	std::printf("\n--LazyGenerator_Coroutine--\n");
	UseCounter();
}


EagerFuture<int> createFuture(int val)
{
	std::puts("generator::generator createFuture(int val) funct");
	co_return val;
}

inline void EagerFuture_Coroutine()
{
	std::printf("\n--EagerFuture_Coroutine--\n");

	auto fut = createFuture(2020);

	fmt::print("eager future result : {}\n", fut.get());
	fmt::print("eager future result : {}\n", fut.get());
}

inline void ByteStreamParser_OLDWAY()
{
	std::printf("\n--ByteStreamParser_OLDWAY--\n");
	const std::vector<std::byte> fakeBytes1{
		std::byte{0x70}, std::byte{0x24}, ESC, SOF, ESC,
		std::byte{'H'}, std::byte{'e'}, std::byte{'l'}, std::byte{'l'}, std::byte{'o'}, 
		ESC, SOF, std::byte{0x7}, ESC, SOF };

	auto frameCompleteHandler = [](std::string_view res) {  fmt::print("CCCB: {} ", res); };
	for (const auto& data : fakeBytes1)
	{
		ProcessNextByte(data, frameCompleteHandler);
	}
	const std::vector<std::byte> fakeBytes2{ std::byte{'W'}, std::byte{'o'}, std::byte{'r'}, std::byte{'l'}, std::byte{'d'}, ESC, SOF, std::byte{0x99} };
	for (const auto& data : fakeBytes2)
	{
		ProcessNextByte(data, frameCompleteHandler);
	}
}

inline void InfinitiDataStream_Cornt()
{
	std::printf("\n--InfinitiDataStream_Cornt--\n");
	auto datagen = getNextData();
	std::printf("\ngetNext(0, 1): \n");
	for (int i =0; i<=4; ++i)
	{
		fmt::print("datagen.next(): {}\n", datagen.next());
		std::printf("Main: datagen.getValue: %d \n", datagen.getValue());
	}

	//std::printf("\ngetNext(100, -10): ");
	//auto datagen2 = getNextData(100, -10);
	//for (int i = 0; i <= 10; ++i)
	//{
	//	datagen2.next();
	//	std::printf("%d ", datagen2.getValue());
	//}
}

inline void LazyFuture_Coroutine()
{
	std::printf("\n--LazyFuture_Coroutine--\n");

	auto fut = createLazyFuture(2020);

	fmt::print("lazy future result : {}\n", fut.get());
}

inline void FutureOnThread_Coroutine()
{
	std::printf("\n-- FutureOnThread_Coroutine--\n");
	fmt::print("Main(): this_thread_id: {}\n", std::this_thread::get_id());
	auto fut = createFutureOnThread(2021);
	fmt::print("lazy future result : {}\n", fut.get());
}

inline void GeneratorWithContainers_Cornt()
{
	std::printf("\n--GeneratorWithContainers_Cornt--\n");

	std::string HelloMe = "Hello Meee!!!";

	auto gen = getNext(HelloMe);
	for (int i = 0; i < HelloMe.size(); ++i)
	{
		fmt::print("{}", gen.getValue());
	}

	std::puts("");

	std::vector vec1{ 1,2,3,4,5 };
	auto gen2 = getNext(vec1);

	for (int i = 0; i < vec1.size(); ++i)
	{
		fmt::print("{}", gen2.getValue());
	}

	std::puts("\nGenNext2 ");

	auto gen3 = getNext(std::span{ vec1});

	for (int i = 0; i < vec1.size(); ++i)
	{
		fmt::print("{}", gen3.getValue());
	}
}

inline void TransparentAwaiter_Cornt()
{
	std::printf("\n--TransparentAwaiter_Cornt--\n");

	auto transparentJob = prepareTransJob();
	transparentJob.start();

	std::puts("\nAfter the job");
}

inline void AutoResumeAwaiter_Cornt()
{
	std::printf("\n--AutoResumeAwaiter_Cornt--\n");

	std::puts("Before Job!");
	
	performJobAutoResume();
	performJobAutoResume();
	performJobAutoResume();
	performJobAutoResume();

}

inline void AwaiterThread_Cornt()
{
	std::printf("\n--AwaiterThread_Cornt--\n");

	std::vector<std::jthread> threadPool(8);
	//for (auto& thread : threadPool) performJobOnThread(thread);

	// just testing to use ranges for fun
	std::ranges::for_each(threadPool, performJobOnThread);
}


inline void CoroutineBasics()
{
	using namespace std::chrono_literals;
	std::printf("\n--CoroutineBasics--\n");

	auto greeting = sayHello();
	std::puts("coroutine initialization started!");

	while (greeting.resume())
	{
		std::this_thread::sleep_for(500ms);
	}

}

inline void CoroutineBasicsYield()
{
	using namespace std::chrono_literals;
	std::printf("\n--CoroutineBasicsYield--\n");

	std::vector<int> vec1{ 1,2,3,4,5 };

	auto genValue = loopOver(vec1);

	std::puts("- start loop from main()");
	while (genValue.resume())
	{
		fmt::print("value: {}\n", genValue.GetValue());
		std::this_thread::sleep_for(1s);
	}
}

inline void CoroutineBasicsIterator()
{
	using namespace std::chrono_literals;
	std::printf("\n--CoroutineBasicsIterator--\n");

	std::vector<int> vec1{ 0, 45, 65, 85, 1 };
	auto gen = loopOverRange(vec1);

	std::puts("- starting to loop over the coroutine!");
	for (const auto& elem : gen)
	{
		fmt::print("value: {} \n", elem);
		std::this_thread::sleep_for(1s);
	}
}

template<typename T>
//void insert(std::vector<T>& coll, const std::type_identity_t<T>& value)
void insert(std::vector<T>& coll, const T& value)
{
	coll.push_back(value);
}

inline void CoroutineBasicsCoReturn()
{
	using namespace std::chrono_literals;
	std::printf("\n--CoroutineBasicsCoReturn--\n");

	auto task = computeinSteps();

	while (task.resume()) 
	{ 
		std::this_thread::sleep_for(500ms);
	}

	fmt::print("result: {}", task.getResult());

}








