#pragma once
#include "ConcurrencyPCH.h"



template<typename T, typename G>
struct promise_type_base
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

namespace coro_iterator
{
	//PT stands for promise type; Iterator is used to get the current and next values
	template<typename PT>  
	struct iterator 
	{
		std::coroutine_handle<PT> mCoroHd1{ nullptr };

		void resume() 
		{ 
			if (not mCoroHd1.done())
			{
				mCoroHd1.resume();
			}
		}
		
		iterator() = default;
		iterator(std::coroutine_handle<PT> hco) : mCoroHd1{ hco } { resume(); }

		void		operator++() { resume(); }
		bool		operator==(const iterator&) const { return mCoroHd1.done(); }
		const auto& operator*() const { return mCoroHd1.promise().mValue; }
	};

} // end of namespace coro_iterator


template<typename T>
struct generator
{
	/* The promise type*/
	using promise_type = promise_type_base <T, generator>;
	using PromiseTypeHandle = std::coroutine_handle<promise_type>;

	/* Make generator iterable*/
	using iterator = coro_iterator::iterator<promise_type>;
	iterator begin() { return { mCoroHd1 }; }
	iterator end() { return {}; }

	generator(const generator&) = delete;
	generator(generator&& rhs) : mCoroHd1( std::exchange(rhs.mCoroHd1, nullptr) ) {}

	~generator()
	{
		/* Coroutine handle keeps a pointer to state of coroutine  
		  therefore if it is not null it should be destroyed */
		if (mCoroHd1) { mCoroHd1.destroy(); }
	}
	
private:
	/* generator constructor is private therefore to access promise_type we need to friend it*/
	friend promise_type;

	explicit generator(promise_type* p) : mCoroHd1{ PromiseTypeHandle::from_promise(*p) } {}


	PromiseTypeHandle mCoroHd1;
};


using IntGenerator = generator<int>;

void UseCounterValue(int i);

/* Coroutine function since it uses co_yield*/
IntGenerator counter(int start, int end)
{
	while (start < end)
	{
		co_yield start;
		++start;
		
	}
}

void UseCounter()
{
	auto g = counter(1, 5);

	for (auto i : g)
	{
		
		UseCounterValue(i);
	}
}

void UseCounterValue(int i)
{
	std::printf("UseCounter value from coroutine: %d\n", i);
}

