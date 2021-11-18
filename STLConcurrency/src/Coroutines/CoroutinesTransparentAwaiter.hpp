#pragma once
#include "ConcurrencyPCH.h"

struct TransSuspendAlways
{
	bool await_ready() const noexcept
	{
		std::puts("TransSuspendAlways::await_ready");
		// indicates an await expression always suspends
		return false;
	}

	void await_suspend(std::coroutine_handle<>) const noexcept
	{
		// does nothing since it is suspend_always
		std::puts("TransSuspendAlways::await_suspend");
	}

	 void await_resume() const noexcept
	{
		// An await expression does not produce a value if suspend_always is used
		std::puts("TransSuspendAlways::await_resume");
	}
};

struct TransSuspendNever
{
	bool await_ready() const noexcept
	{
		std::puts("TransSuspendNever::await_ready (never suspends)");
		// indicates an await expression never suspends
		return true;
	}

	void await_suspend(std::coroutine_handle<>) const noexcept
	{
		// does nothing since it is suspend_always
		std::puts("TransSuspendNever::await_suspend");
	}

	void await_resume() const noexcept
	{
		// An await expression does not produce a value if suspend_never is used
		std::puts("TransSuspendNever::await_resume");
	}
};

struct JobTransparent
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	handle_type coro_handle;
	JobTransparent(handle_type h) :coro_handle{ h } {}
	~JobTransparent()
	{
		if (coro_handle) coro_handle.destroy();
	}

	void start() 
	{ 
		std::puts("JobTransParent::start()");
		coro_handle.resume(); 
	}

	struct promise_type
	{
		auto get_return_object() { return JobTransparent{ handle_type::from_promise(*this) }; }
		
		auto initial_suspend() 
		{ 
			std::puts("Job prepared");
			return TransSuspendAlways{}; 
		}

		auto final_suspend() noexcept
		{
			std::puts("Job finished");
			return TransSuspendAlways{};
		}

		void return_void()			{}
		void unhandled_exception()	{}
	};
};

inline JobTransparent prepareTransJob()
{
	co_await TransSuspendNever();
}