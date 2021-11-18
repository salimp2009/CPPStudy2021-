#pragma once
#include "ConcurrencyPCH.h"

template<typename T>
struct EagerFuture
{
	std::shared_ptr<T> value;
	EagerFuture(std::shared_ptr<T> p) : value(p) { std::puts("  EagerFuture::Eager Future Constructor"); }
	~EagerFuture() { std::puts(" EagerFuture::Eager Future Destructor"); }
	T get()
	{
		std::puts(" EagerFuture::Eager Future Get() funct");
		return *value; 
	}

	struct promise_type
	{
		std::shared_ptr<T> ptr = std::make_shared<T>();
		promise_type() { std::puts("	promise_type::promise_type Constructor"); }
		~promise_type() { std::puts("	promise_type::promise_type Destructor"); }
		EagerFuture<T> get_return_object()
		{
			std::puts("	promise_type::promise_type get_return_object() funct");

			return ptr;
		}

		void return_value(T v) 
		{ 
			std::puts("	promise_type::promise_type return_value(T v)");
			*ptr = v; 
		} /* since this is eager; not suspending!*/
		
		std::suspend_never initial_suspend() 
		{
			std::puts("	promise_type::promise_type initial_suspend() funct");
			return {}; 
		}
		std::suspend_never final_suspend() noexcept 
		{ 
			std::puts("	promise_type::promise_type final_suspend() funct");
			return {}; 
		}

		void unhandled_exception() 
		{
			std::puts("		promise_type::promise_type unhandled_exception() funct");
			std::exit(1); 
		}
	};
};




