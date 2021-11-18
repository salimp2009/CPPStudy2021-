#pragma once
#include "ConcurrencyPCH.h"

/* Not complete !!!!*/

template<typename T, typename G>
struct promise_type
{
	/* value yielded from a coroutine*/
	T mValue;

	/* invoked by co_yield or co_return*/
	auto yield_value(T value)
	{
		/* storing the value for access outside the coroutine*/
		mValue = std::move(value);
		/* Suspend the coroutine*/
		return std::suspend_always{};
	}

	/*return the generator*/
	G get_return_object() { return G{ this }; }

	std::suspend_always initial_suspend() { return {}; }
	std::suspend_always final_suspend() noexcept { return {}; }

	void return_void() {}
	void unhandled_exception() { std::terminate(); }
	static auto get_return_object_on_allocation_failure() { return G{ nullptr }; }

};

template<typename T>
struct Generator
{
	using Promise = promise_type<T, Generator>;
	using handle_type = std::coroutine_handle<Promise>;

	Generator(handle_type handle) : coro(handle) { }
	handle_type coro;

	~Generator()
	{
		if (coro) coro.destroy();
	}

	T getvalue() { return coro.promise().mValue; }

	bool next()
	{
		coro.resume();
		return not coro.done();
	}

};
