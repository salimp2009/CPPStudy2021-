#pragma once
#include "ConcurrencyPCH.h"



template<typename CoroType>
struct promisetypebase
{
	using handle_type = std::coroutine_handle<promisetypebase<CoroType>>;

	auto get_return_object()
	{
		std::puts("- promise : get_return_object()");
		return handle_type::from_promise(*this);
	}

	auto initial_suspend()
	{
		std::puts("- promise::initial_suspend");
		return std::suspend_always();
	}

	auto final_suspend() noexcept
	{
		std::puts("- promise::final_suspend");
		return std::suspend_always();
	}

	void return_void() 
	{	// deal with end or with return 
		std::puts("- promise::return_void()");
	}

	void unhandled_exception()
	{
		std::terminate();
	}
};

class CoroTask
{
public:
	using promise_type = promisetypebase<CoroTask>;
	using handle_type = std::coroutine_handle<promise_type>;
private:
	handle_type coroHandle;
public:
	CoroTask(handle_type h) : coroHandle{ h } { std::puts("- CoroTask::constructor!"); }
	~CoroTask()
	{
		std::puts("- CoroTask::destructor");
		if (coroHandle) coroHandle.destroy();
	}

	// these deletes both copy and move constructor/assignments
	CoroTask(const CoroTask&) = delete;
	CoroTask& operator=(const CoroTask&) = delete;

	bool resume() const
	{
		std::puts("- CoroTask::resume()");
		// nothing to process
		if (!coroHandle) { return false; }
		coroHandle.resume();
		return ! coroHandle.done();
	}
};

inline CoroTask sayHello()
{
	std::puts("Hello");
	co_await std::suspend_always();
	std::puts("coroutine");
	co_await std::suspend_always();
	std::puts("basics!");
}

