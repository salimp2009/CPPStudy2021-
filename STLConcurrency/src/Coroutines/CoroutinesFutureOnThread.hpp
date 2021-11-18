#pragma once
#include "ConcurrencyPCH.h"

template<typename T>
struct FutureOnThread
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;

	handle_type coro_handle;

	FutureOnThread(handle_type h) :coro_handle{ h } { std::puts("	FutureOnThread::Constructor"); }
	~FutureOnThread()
	{
		std::puts("FutureOnThread::Destructor");
		if (coro_handle) coro_handle.destroy();
	}

	T get()
	{
		std::puts("	FutureOnThread::get");
		fmt::print("	this_thread_id: {}\n", std::this_thread::get_id());

		//if std::jthread is used then the client may ask result before thread joins
		// Soln: Create a temp jthread or call on jthread.join() manually as in regular thread !! 
		std::thread futureThread{ [this]() 
		{
				fmt::print("futureThread: this_thread_id: {}\n", std::this_thread::get_id());
				coro_handle.resume(); } 
		};
		futureThread.join();
		return coro_handle.promise().result;
	}

	struct promise_type
	{
		T result;

		// typically use default constructor ; this example used user defined constructor to see the steps involved !!
		promise_type() 
		{ 
			std::puts("	promise_type::Constructor"); 
			fmt::print("	this_thread_id: {}\n", std::this_thread::get_id());
			
		}
		~promise_type() 
		{ 
			std::puts("	promise_type::Destructor"); 
			fmt::print("	this_thread_id: {}\n", std::this_thread::get_id());
		}

		auto get_return_object()
		{
			std::puts("	promise_type::get_return_object");
			return FutureOnThread{ handle_type::from_promise(*this) };
		}

		void return_value(T v)
		{
			std::puts("	promise_type::return_value");
			fmt::print("	this_thread_id: {}\n", std::this_thread::get_id());
			result = v;
		}

		auto initial_suspend()
		{
			return std::suspend_always{};
		}

		auto final_suspend() noexcept
		{
			std::puts("	promise_type::final_suspend");
			fmt::print("	this_thread_id: {}\n", std::this_thread::get_id());
			return std::suspend_always{};
		}

		void unhandled_exception()
		{
			std::exit(1);
		}

	};
};


FutureOnThread<int> createFutureOnThread(int n)
{
	std::puts(" createFutureOnThread");
	co_return n;
}


