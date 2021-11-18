#pragma once
#include "ConcurrencyPCH.h"


std::random_device seed3;

auto numberGenerator2 = []()
{
	std::default_random_engine eng2(seed3());
	std::uniform_int_distribution<>distrib2(0, 1);
	return  distrib2(eng2);
};

struct AwaitableThread
{
	std::jthread& outerThread;

	bool await_ready() const noexcept
	{
		auto result = numberGenerator2();
		if (result)
		{
			std::puts("(executed!)");
		}
		else
		{
			std::puts("(suspended!)");
		}
		return result;
	}

	void await_suspend(std::coroutine_handle<>h)
	{
		outerThread = std::jthread([h]() 
		{ 
				//fmt::print("await_suspend: {}\n", std::this_thread::get_id());
				h.resume(); }
		);
	}

	void await_resume(){}
};

struct JobMultiThread
{
	static inline int JobCounter{ 1 };

	//struct promise_type;
	//using handle_type = std::coroutine_handle<promise_type>;
	//handle_type coroHandle;

	JobMultiThread() { ++JobCounter; }
	//JobMultiThread(handle_type h) : coroHandle{ h } {}
	//~JobMultiThread() { if (coroHandle) coroHandle.destroy(); }

	struct promise_type
	{
		int JobNumber{ JobCounter };

		JobMultiThread get_return_object() 
		{ 
			std::puts("promise_type::get_return_object()");
			return {}; 
		}
		
		std::suspend_never initial_suspend()
		{
			fmt::print("Job: {0}, prepared on thread {1}\n", JobNumber, std::this_thread::get_id());
			return {};
		}

		std::suspend_never final_suspend() noexcept
		{
			fmt::print("Job: {0}, finished on thread {1}\n", JobNumber, std::this_thread::get_id());
			return {};
		}

		void return_void()	{}
		void unhandled_exception() {}
	};
};


inline JobMultiThread performJobOnThread(std::jthread& outThread)
{
	co_await AwaitableThread(outThread);
}