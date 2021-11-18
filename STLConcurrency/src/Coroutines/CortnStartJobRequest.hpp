#pragma once
#include "ConcurrencyPCH.h"

struct Job
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	handle_type coro;

	Job(handle_type h) : coro{ h } {}
	~Job() { if (coro) { coro.destroy(); } }

	void start() { coro.resume(); }

	struct promise_type
	{
		auto get_return_object() 
		{ 
			return Job{ handle_type::from_promise(*this) }; 
		}

		std::suspend_always initial_suspend() 
		{ 
			std::printf("Preparing Job: initial suspend!\n");
			return {}; 
		}

		std::suspend_always final_suspend() noexcept
		{
			std::printf("Finishing Job: final suspend!\n");
			return {};
		}

		void return_void() {}
		void unhandled_exception() {}
	};
};

Job prepareJob() { co_await std::suspend_never(); }

inline void Cortn_StartJobRequest()
{
	std::printf("\n--Cortn_StartJobRequest--\n");

	std::printf("Before Job!\n");

	auto job1 = prepareJob();
	job1.start();

	std::printf("After Job!\n");
}