#pragma once
#include "ConcurrencyPCH.h"


template<typename T>
struct LazyFuture
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;

	handle_type coro_handle;

	LazyFuture(handle_type h) :coro_handle{ h } { std::puts("	Lazy Future::Constructor"); }
	~LazyFuture()
	{
		std::puts("	Lazy Future::Destructor");
		if (coro_handle) coro_handle.destroy();
	}

	T get()
	{
		std::puts("	Lazy Future::get");
		coro_handle.resume();
		return coro_handle.promise().result;
	}

	struct promise_type
	{
		T result;

		// typically default constructor ; this example used user defined to see the steps involved !!
		promise_type()  { std::puts("	promise_type::Constructor"); }
		~promise_type() { std::puts("	promise_type::Destructor"); }

		auto get_return_object()
		{
			std::puts("	promise_type::get_return_object");
			return LazyFuture{ handle_type::from_promise(*this) };
		}

		void return_value(T v)
		{
			std::puts("	promise_type::return_value");
			result = v;
		}

		auto initial_suspend()
		{
			std::puts("	promise_type::initial_suspend");
			return std::suspend_always{};
		}

		auto final_suspend() noexcept
		{
			std::puts("	promise_type::final_suspend");
			return std::suspend_always{};
		}

		void unhandled_exception()
		{
			std::exit(1);
		}

	};
};


LazyFuture<int> createLazyFuture(int n)
{
	std::puts(" createLazyFuture");
	co_return n;
}

