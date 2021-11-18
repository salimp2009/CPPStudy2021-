#pragma once
#include "ConcurrencyPCH.h"

class IntGen
{
public:
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;

public:
	IntGen(handle_type h):coroHandle{h} {}
	~IntGen() { if (coroHandle) coroHandle.destroy(); }

	// no copying but move is OK
	IntGen(const IntGen&) = delete;
	IntGen(IntGen&& rhs) noexcept : coroHandle{ std::exchange(rhs.coroHandle, nullptr) } {}
	IntGen& operator=(const IntGen&) = delete;

	bool resume() const
	{
		if (!coroHandle) 
		{ 
			return false; 
		}
		coroHandle.resume();
		return !coroHandle.done();
	}

	int GetValue() const { return coroHandle.promise().currentValue; }


private:
	handle_type coroHandle;

public:
	struct promise_type
	{
		int currentValue;

		auto get_return_object()		{ return handle_type::from_promise(*this); }
		auto initial_suspend()			{ return std::suspend_always{}; }
		auto final_suspend() noexcept	{ return std::suspend_always{}; }

		// called when co_yield is used
		auto yield_value(int value)
		{
			// store the value from co_yield instruction
			currentValue = value;
			return std::suspend_always{};
		}

        void return_void() {}
        void unhandled_exception()	{ std::terminate(); }
	};
	
};

template<typename T>
inline IntGen loopOver(const T& coll)
{
	for (int elem : coll)
	{
		std::puts("- suspend");
		co_yield elem;
		std::puts("- resume");
	}
}
