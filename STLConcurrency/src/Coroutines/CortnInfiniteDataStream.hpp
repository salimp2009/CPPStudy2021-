#pragma once
#include "ConcurrencyPCH.h"

template<typename T>
struct DataGenerator
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;

	DataGenerator(handle_type h) : coro{ h } 
	{ 
		//std::puts("	Generator::Constructor"); 
	}
	handle_type coro;

	~DataGenerator() 
	{ 
		//std::puts("	Generator::Destructor");
		if (coro) { coro.destroy(); } 
	}
	
	DataGenerator(const DataGenerator&) = delete;
	DataGenerator& operator=(const DataGenerator&) = delete;
	
	DataGenerator(DataGenerator&& other) noexcept: coro{std::exchange(other.coro, nullptr)} { }
	DataGenerator& operator=(DataGenerator&& other) noexcept
	{
		coro = std::exchange(other.coro, nullptr);
		return *this;
	}

	T getValue() 
	{ 
		coro.resume();
		//std::puts("	Generator::GetValue");
		return coro.promise().current_value; 
	}

	bool next()
	{
		//std::puts("	Generator::next()");
		coro.resume();
		return not coro.done();
	}

	struct promise_type
	{
		// typically use default constructor & destructor ; this example used user defined constructor to output/debug the steps involved !!
		promise_type() 
		{ 
			//std::puts("	promise_type::Constructor()"); 
		}
		~promise_type() 
		{ 
			//std::puts("	promise_type::Destructor()"); 
		}

		auto initial_suspend()        
		{
			//std::puts("	promise_type::initial_suspend"); 
			return std::suspend_always{}; 
			// Testing different case to see the effect on work flow; this cause the initial suspend does not happen
			// and the initial value is no yielded to generator nor the main; the value start from the next
			//return std::suspend_never{};
		}
		auto final_suspend() noexcept 
		{
			//std::puts("	promise_type::final_suspend");
			return std::suspend_always{}; 
		}

		auto get_return_object() 
		{
			//std::puts("	promise_type::get_return_object");
			return DataGenerator{ handle_type::from_promise(*this) }; 
		}
		auto return_void()		 { return std::suspend_never{}; }

		auto yield_value(const T& value)
		{
			//std::puts("	promise_type::yield_value");
			current_value = value;
			return std::suspend_always{};

			// Testing different case to see the effect on work flow; this causes the value will never be passed & coroutine runs forever
			//return std::suspend_never{};
		}

		void unhandled_exception() { std::exit(1); }

		T current_value;
	};
};

inline DataGenerator<int> getNextData(int start = 0, int step = 1)
{
	std::puts("	getNextData::start");
	auto value = start;
	while (true)
	{
		std::puts("	getNextData::before co_yield");
		co_yield value;
		std::puts("	getNextData::after co_yield");
		value += step;
	}
}

template<typename Cont>
inline DataGenerator<typename Cont::value_type> getNext(Cont cont)
{
	for (auto value : cont)
	{
		co_yield value;
	}
}

template<typename T, std::size_t N>
inline DataGenerator<T> getNext(std::span<T, N> cont)
{
	std::puts("getNext with span");
	for (auto&& value : cont)
	{
		co_yield value;
	}
}