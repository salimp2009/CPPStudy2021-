#pragma once
#include "ConcurrencyPCH.h"

std::random_device seed2;

auto numberGenerator = []()
{
	std::default_random_engine eng(seed2());
	std::uniform_int_distribution<>distrib(0, 1) ;
	return  distrib(eng);
};

struct AwaitableAutoResume
{
	bool await_ready() const noexcept
	{
		std::puts("	AwaitableAutoResume::await_ready");
		return numberGenerator();
	}

	bool await_suspend(std::coroutine_handle<> handle) const noexcept
	{
		std::puts("	AwaitableAutoResume::await_suspend");
		handle.resume();
		return true;
	}

	void await_resume() const noexcept
	{
		// An await expression does not produce a value if suspend_always is used
		std::puts("	AwaitableAutoResume::await_resume");
	}
};


struct JobAutoResume
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	handle_type coro_handle;

	JobAutoResume(handle_type h):coro_handle{h} {}
	~JobAutoResume()
	{
		if (coro_handle) coro_handle.destroy();
	}

	struct promise_type
	{
		auto get_return_object() { return JobAutoResume{ handle_type::from_promise(*this) }; }

		auto initial_suspend()
		{
			std::puts("Job prepared!");
			return AwaitableAutoResume{};
		}

		auto final_suspend() noexcept
		{
			std::puts("Job finished!");
			return std::suspend_always{};
		}

		void return_void() {}
		void unhandled_exception() {}
	};

};

inline JobAutoResume   performJobAutoResume()
{
	co_await std::suspend_never{};
}

