#pragma once

inline void StopToken_Example()
{
	std::printf("\n---------------Stop Token Basics---------------------\n");

	using namespace std::literals;
	auto func = [](std::stop_token stoken) 
	{
		std::atomic<int>counter{ 0 };

		auto threadID = std::this_thread::get_id();
		std::stop_callback callBack(stoken, [&counter, threadID]()
		{
				fmt::print("thread id: {0}, counter: {1}\n", threadID, counter);

		});

		while (counter < 10)
		{
			std::this_thread::sleep_for(0.2s);
			++counter;
		}
	};

	std::vector<std::jthread> vecThreads(10);

	for (auto& thr : vecThreads)
	{
		thr = std::jthread(func);
	}

	/* allows time for threads to increase counter before request_stop is called 
	   before counter reaches to condition counter<10
	*/
	std::this_thread::sleep_for(1s);

	for (auto& thr : vecThreads)
	{
		thr.request_stop();
	}
}


inline void StopToken_ConditionVar()
{
	std::printf("\n---------------StopToken with Condition Var---------------------\n");

	/* this example used mutex and locks but could be done better with atomic bool or atmoic_flag
		and use store and load with acquire and release memory order semantics; will be more efficient for real case!!
	*/

	using namespace std::literals;

	std::mutex mut;
	std::condition_variable_any condVar;

	bool dataReady{ false };

	auto receiver = [&](std::stop_token stoken)
	{
		fmt::print("Receiver: waiting!\n");
		std::unique_lock<std::mutex> lck{ mut };
		bool ret = condVar.wait(lck, stoken, [&dataReady]() {return dataReady; });
		if (ret)
		{
			fmt::print("Notification received: dataReady {}\n", dataReady);
		}
		else
		{
			fmt::print("Stop requested!\n", dataReady);
		}
	};

	auto sender = [&]()
	{
		std::this_thread::sleep_for(5ms);
		{
			std::lock_guard<std::mutex>lck(mut);
			dataReady = true;
			fmt::print("Sender: Data Ready!!\n");
		}
		condVar.notify_one();
	};

	std::jthread jth1{ receiver };
	std::jthread jth2{ sender };

	/* sending a stop request before sender sends a notification that data is ready
	   therefore the receiver will not get the data !!
	*/
	jth1.request_stop();

	jth1.join();
	jth2.join();

}

inline void JThread_Basics()
{
	std::printf("\n---------------JThread_Basics---------------------\n");
	using namespace std::literals;

	std::jthread nonInterruptible([]() noexcept
		{
			int counter{ 0 };
			while (counter < 10)
			{
				std::this_thread::sleep_for(0.2s);
				fmt::print("Non Intterruptible: {}\n", counter);
				++counter;
			}
		});

	std::jthread interruptible([](std::stop_token stoken) noexcept
		{
			int counter{ 0 };
			while (counter < 10)
			{
				std::this_thread::sleep_for(0.2s);
				if (stoken.stop_requested()) return;
				fmt::print("Intterruptible: {}\n", counter);
				++counter;
			}
		});


	std::this_thread::sleep_for(1s);
	fmt::print("\nMain thread requesting stops for both threads!!\n\n");
	nonInterruptible.request_stop();
	interruptible.request_stop();

}


void func2(std::stop_token st, int num)
{
	using namespace std::literals;

	auto id = std::this_thread::get_id();
	fmt::print("call func: {}\n", num);

	std::stop_callback cb1(st, [num, id]()
		{
			fmt::print("--STOP1 requested in func: {} {}\n", num, (id == std::this_thread::get_id() ? "\n" : "in main thread"));
		});

	std::this_thread::sleep_for(9ms);

	std::stop_callback cb2(st, [num, id]()
		{
			fmt::print("--STOP2 requested in func: {} {}\n", num, (id == std::this_thread::get_id() ? "\n" : "in main thread"));
		});

	std::this_thread::sleep_for(2ms);
}

inline void StopSource_Basics()
{
	std::printf("\n-StopSource_Basics(-\n");
	using namespace std::literals;

	std::stop_source ssrc;
	std::stop_token stoken{ ssrc.get_token() };

	std::stop_callback cb{ stoken, []() 
	{
		std::puts("STOP requested from main()");
	} };

	auto fut = std::async([stoken]() 
	{
			for (int num = 1; num < 10; ++num)
			{
				func2(stoken, num);
			}
	});

	std::this_thread::sleep_for(120ms);
	ssrc.request_stop();

}


