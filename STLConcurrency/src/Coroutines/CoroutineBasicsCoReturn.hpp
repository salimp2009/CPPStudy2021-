#pragma once
#include "ConcurrencyPCH.h"


class StringTask
{
public:
	struct promise_type;
	using HandleType = std::coroutine_handle<promise_type>;

private:
	HandleType coroHandle;
public:

	StringTask(HandleType h): coroHandle{h}{ }
	~StringTask() { if (coroHandle)  coroHandle.destroy();  }

	// no copy or move allowed
	StringTask(const StringTask&) = delete;
	StringTask& operator=(const StringTask&) = delete;

	bool resume() const 
	{
		if (!coroHandle)
		{
			return false;
		}
		coroHandle.resume();
		return !coroHandle.done();
	}

	std::string getResult() const
	{
		return coroHandle.promise().result;
	}

	struct promise_type
	{
		std::string result;

		auto get_return_object()
		{
			return HandleType::from_promise(*this);
		}	
		
		auto initial_suspend()			{ return std::suspend_always{}; }
		auto final_suspend() noexcept	{ return std::suspend_always{}; }
		void return_value(const auto& value) { result = value; }
		void unhandled_exception() { std::terminate(); }
	};

};

inline StringTask computeinSteps()
{
	std::string ret;
	ret += "Hello ";
	co_await std::suspend_always();
	ret += "Didem";
	co_await std::suspend_always();
	ret += '!';
	co_return ret;
}