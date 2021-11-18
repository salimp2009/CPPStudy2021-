#pragma once

#include "ConcurrencyUtility.hpp"


inline void FireForgetFutures()
{
	std::printf("\n------FireForgetFutures------\n");
/* if run this way it runs as a sequential since the std::async holds on it destructpr and block
	and also you will get a return value is no discard 
*/
  auto fut1 = std::async(std::launch::async, []()
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			fmt::print("first thread!!\n");
		});


 auto fut2 = std::async(std::launch::async, []()
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			fmt::print("second thread!!\n");
		});

	fmt::print("main thread!!\n");
}


inline void dotProductAsync()
{
	std::printf("\n------ dot Product with Async------\n");

	static const int NUM = 100;

	//auto dotProduct = [](const std::vector<int>& v, const std::vector<int>& w) noexcept
	/* ALTERNATIVE Using std::span ; C++20*/
	auto dotProduct = [](std::span<int> v, std::span<int> w) noexcept

	{
		auto vSize = v.size();

		auto future1 = std::async([&] {
			//return std::inner_product(&v[0], &v[vSize / 4], &w[0], 0LL);
			return std::inner_product(v.begin(), v.begin() + vSize / 4, w.begin(), 0LL);
			});

		auto future2 = std::async([&] {
			//return std::inner_product(&v[vSize / 4], &v[vSize / 2], &w[vSize / 4], 0LL);
			return std::inner_product(v.begin() + vSize / 4, v.begin() + vSize / 2, w.begin() + vSize / 4, 0LL);
			});

		auto future3 = std::async([&] {
			//return std::inner_product(&v[vSize / 2], &v[vSize * 3 / 4], &w[vSize / 2], 0LL);

			return std::inner_product(v.begin() + vSize / 2, v.begin() + vSize * 3 / 4, w.begin() + vSize / 2, 0LL);
			});

		auto future4 = std::async([&] {
			//return std::inner_product(&v[vSize * 3 / 4], &v[vSize], &w[vSize * 3 / 4], 0LL);

			return std::inner_product(v.begin() + vSize * 3 / 4, v.end(), w.begin() + vSize * 3 / 4, 0LL);

			});

		return future1.get() + future2.get() + future3.get() + future4.get();

	};

	std::random_device seed;
	std::mt19937 engine(seed());
	std::uniform_int_distribution<int> dist(0, 100);

	std::vector<int> v, w;
	v.reserve(NUM);
	v.reserve(NUM);

	for (auto i = 0; i < NUM; ++i)
	{
		v.push_back(dist(engine));
		w.push_back(dist(engine));
	}
	std::cout<<"dot product : "<<dotProduct(v, w)<<'\n';

	/* NOTE: this is for testing a bug in std::inner_procut implementation of VS 2019 16.9.4
		the end of the container gives exception error if passed with &vec1[vec1.size()]
		and if &vec1[vec1.size()-1] then it does not take the last elements into account!
		reported VS Developer web site; 
	*/
	//std::vector<int> vec1 = { 1,2,3,4 };
	//std::vector<int> vec2 = { 1,2,3,4 };

	//auto result = std::inner_product(&vec1[0], &vec1[vec1.size()], &vec2[0], 0);

	//std::cout << result << '\n';

}



inline void PackagedTask_Basics()
{
	std::printf("\n------ Packaged Task_Basics------\n");

	
	auto sumUp = [](int beg, int end) 
	{
		long long int sum{ 0 };
		for (int i = beg; i < end; ++i)
		{
			sum += i;
		}
		return sum;
	};


	std::packaged_task<long long(int, int)> sumTask1(sumUp);
	std::packaged_task<long long(int, int)> sumTask2(sumUp);
	std::packaged_task<long long(int, int)> sumTask3(sumUp);
	std::packaged_task<long long(int, int)> sumTask4(sumUp);

	auto fut1 = sumTask1.get_future();
	auto fut2 = sumTask2.get_future();
	auto fut3 = sumTask3.get_future();
	auto fut4 = sumTask4.get_future();

	std::deque<std::packaged_task<long long(int,int)>> allTasks;

	allTasks.push_back(std::move(sumTask1));
	allTasks.push_back(std::move(sumTask2));
	allTasks.push_back(std::move(sumTask3));
	allTasks.push_back(std::move(sumTask4));

	int begin{ 1 };
	int increment{ 2500 };
	int end = begin + increment;

	while (not allTasks.empty())
	{
		auto myTask = std::move(allTasks.front());
		allTasks.pop_front();
		std::thread sumThread(std::move(myTask), begin, end);
		begin = end;
		end += increment;
		sumThread.detach();
	}

	auto sum = fut1.get() + fut2.get() + fut3.get() + fut4.get();

	fmt::print("sum of 0...10'000: {}\n", sum);

}

inline void PackagedTask_Reuse()
{
	std::printf("\n------ Packaged Task Reuse------\n");

	auto calcProducts = [](std::packaged_task<int(int, int)>& task, const std::vector<std::pair<int, int>>& pairs)
	{
		fmt::print("thread id {}\n", std::this_thread::get_id());
		for (auto& [first, second] : pairs)
		{
			auto fut = task.get_future();
			task(first, second);
			fmt::print("first: {0}, second: {1} = {2}\n", first, second, fut.get());
			/* once we use the task we reset to use again with the next task*/
			task.reset();
		}
		std::printf("\n");
	};

	std::vector<std::pair<int, int>> allPairs;
	allPairs.emplace_back(1, 2);
	allPairs.emplace_back(2, 3);
	allPairs.emplace_back(3, 4);
	allPairs.emplace_back(4, 5);
	allPairs.emplace_back(7, 8);

	std::packaged_task<int(int, int)> task{ [] (int first, int second)
		{
			return first * second;
		} };

	/* send by main thread*/
	calcProducts(task, allPairs);

	/* child thread*/
	std::thread t(calcProducts, std::ref(task), std::ref(allPairs));

	t.join();
}


inline void PromiseFuture_Basics()
{
	std::printf("\n-----Promise Future Basics-----\n");

	auto product = [](std::promise<int>&& intPromise, int aa, int bb)
	{
		intPromise.set_value(aa * bb);
	};


	auto div = []<typename T> (std::promise<int>&& intPromise,	T aa, T bb) 
	requires requires(T aa, T bb) { (std::is_same_v<T, int> && bb!=0); }
	{
		intPromise.set_value(aa /  bb);
	};

	int xa = 20;
	int xb = 10;

	std::promise<int> prodPromise;
	std::promise<int> divPromise;

	auto prodResult = prodPromise.get_future();
	auto divResult = divPromise.get_future();

	std::thread prodThread(product, std::move(prodPromise), xa, xb);
	std::thread divThread(div, std::move(divPromise), xa, xb);

	fmt::print("prod : {}\n", prodResult.get());
	fmt::print("div : {}\n", divResult.get());
	
	prodThread.join();
	divThread.join();

}


inline void WaitFor_Basics()
{
	std::printf("\n-----Promise Future Basics-----\n");

	using namespace std::literals::chrono_literals;

	auto getAnswer = [](std::promise<int>&& intPromise)
	{
		std::this_thread::sleep_for(3s);
		intPromise.set_value(42);
	};

	std::promise<int> answerPromise;
	auto fut = answerPromise.get_future();

	std::thread answerThread(getAnswer, std::move(answerPromise));

	std::future_status status;
	do 
	{
		status = fut.wait_for(0.2s);
		std::printf("...doing somethng else\n");
	} while (status != std::future_status::ready);

	
	fmt::print("The answer is: {}", fut.get());

	answerThread.join();
}




inline void SharedFuturePromise_Basics()
{
	std::printf("\n-----Shared Future Promise Basics-----\n");

	/* ONLY to prevent interleaving of output*/
	std::mutex coutMut;

	auto div = []<typename T>(std::promise<T> && inPromise, T a, T b)  requires std::integral<T> || std::floating_point<T>
	{
		assert(b != 0);
		inPromise.set_value(a / b);
	};

	auto requestor = [&]<typename T>(std::shared_future<T> shaFut) requires std::integral<T> || std::floating_point<T>
	//auto requestor = [&](std::shared_future<int> shaFut) 
	{
		std::lock_guard<std::mutex> coutGuard(coutMut);

		fmt::print("thread ID:({0}): {1}\n", std::this_thread::get_id(), shaFut.get());

	};


	std::promise<int> divPromise;
	std::shared_future divResult = divPromise.get_future();

	// calculate division in a seperate thread
	std::thread divThread(div, std::move(divPromise), 20, 10);

	// send the result to other threads
	std::thread reqThread1(requestor, divResult);
	std::thread reqThread2(requestor, divResult);
	std::thread reqThread3(requestor, divResult);
	std::thread reqThread4(requestor, divResult);

	divThread.join();

	reqThread1.join();
	reqThread2.join();
	reqThread3.join();
	reqThread4.join();

}