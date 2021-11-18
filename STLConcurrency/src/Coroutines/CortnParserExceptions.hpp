#pragma once
#include "ConcurrencyPCH.h"
#include <memory_resource>

// THE GLOBAL VARIABLES ARE ALREADY DEFINED IN ANOTHER FILE!
//static  const std::byte ESC{ 'H' };
//static const std::byte SOF{ 0x10 };

// overloading literals; user defined literals https://en.cppreference.com/w/cpp/language/user_literal
	//std::byte operator""_B(char c)
	//{
	//	return static_cast<std::byte>(c);
	//}

	//std::byte operator""_B(unsigned long long  c)
	//{
	//	return static_cast<std::byte>(c);
	//}


namespace CortnExcept
{
	enum class Ex
	{
		None, ParseBegin, ParseWhileRunning, InitialSuspend, YieldValue, GetReturnObject
	};

	static Ex defExcept{ Ex::None };
	static int rethrow = 1;

	void ThrowIf(const Ex exceptValue, std::string name)
	{
		if (exceptValue==defExcept) throw std::runtime_error(name);
	}

	// Initial susoend control whther the coroutine suspends directly after first creation or
	// runs until a co_yield or co_return or co_await statement
	/* the reason for this in this example the Parse needs to run*/
	template<typename T, typename G, bool InitialSuspend>
	struct promise_type_base
	{
		// mValue is the value that will be returned to the caller by yield_value
		T mValue;

		/* invoked by co_yield or co_return*/
		std::suspend_always yield_value(T value)
		{
			std::printf("promise:yield_value!\n");
			ThrowIf(Ex::YieldValue, "promise: yield_value");
			mValue = value;
			return {};
		}

		auto initial_suspend()
		{
			std::printf("promise:initial_suspend!\n");
			ThrowIf(Ex::InitialSuspend, "promise: initial_suspend");
			if constexpr (InitialSuspend)
			{
				return std::suspend_always{};
			}
			else
			{
				return std::suspend_never{};
			}
		}
		std::suspend_always final_suspend() noexcept { return {}; }

		G get_return_object()
		{
			std::printf("promise:get_return_object!\n");
			ThrowIf(Ex::GetReturnObject, "promise: get_return_object");
			return G{ this };
		}

		void return_void() {}
		void unhandled_exception() 
		{ 
			std::printf("unhandled_exception\n");
			if (rethrow)
			{
				if (rethrow == 2)
				{
					auto exceptionPtr = std::current_exception();
					try 
					{
						if (exceptionPtr)  std::rethrow_exception(exceptionPtr);
					}
					catch (const std::exception& e)
					{
						std::printf("promise UnHandled_exception: Exception caught %s", e.what());
					}
				}
				else
				{
					throw;
				}
			}
			else
			{
				std::printf("do nothing\n");
			}
		}
	};

	namespace coro_iterator
	{
		template<typename PT>
		struct iterator
		{
			using coro_handle = std::coroutine_handle<PT>;
			coro_handle mCoroHd1{};

			using RetType = decltype(mCoroHd1.promise().mValue);
			
			void resume(){ mCoroHd1.resume();}

			iterator() = default;
			iterator(coro_handle hco) : mCoroHd1{ hco } { resume(); }

			void operator++()
			{ 
				std::printf("operator++()\n");
				resume(); 
			}

			bool operator==(const iterator&) const { return mCoroHd1.done(); }
			const RetType& operator*()  const& { return mCoroHd1.promise().mValue; }
		};

	} // end of namespace coro_iterator

	template<typename T, bool InitialSuspend = true>
	struct generator
	{
		using promise_type = promise_type_base<T, generator, InitialSuspend>;
		using PromiseTypeHandle = std::coroutine_handle<promise_type>;
		using iterator = coro_iterator::iterator<promise_type>;

		iterator begin() { return mCoroHd1; }
		iterator end()   { return {}; }

		generator(const generator&) = delete;
		generator(generator&& rhs) : mCoroHd1{ std::exchange(rhs.mCoroHd1, nullptr) } {}

		~generator() { if (mCoroHd1) mCoroHd1.destroy(); }

		T operator()()
		{
			T tmp{};
			// use swap for a potential move and defined / cleared state
			std::swap(tmp, mCoroHd1.promise().mValue);
			return tmp;
		}

	private:
		friend promise_type;
		explicit generator(promise_type* p) : mCoroHd1{ PromiseTypeHandle::from_promise(*p) } {}
	protected:
		PromiseTypeHandle mCoroHd1;
	};


	class DataStreamReader
	{
	public:
		DataStreamReader() = default;

		// disabling the move assignment operator ; this also disables the default copy/move operations
		DataStreamReader& operator=(DataStreamReader&&) noexcept = delete;

		struct Awaiter
		{
			Awaiter& operator=(Awaiter&&)    noexcept = delete;
			Awaiter(DataStreamReader& event) noexcept : mEvent{ event }
			{
				mEvent.mAwaiter = this;
			}

			bool await_ready() const noexcept
			{
				return mEvent.mData.has_value();
			}

			void await_suspend(std::coroutine_handle<> coroHd1) noexcept
			{
				mCoroHd1 = coroHd1;
			}

			std::byte await_resume() noexcept
			{
				//std::printf("Awaiter:await resume!\n");
				assert(mEvent.mData.has_value());
				return *std::exchange(mEvent.mData, std::nullopt);
			}

			void resume() { mCoroHd1.resume(); }

		private:
			DataStreamReader& mEvent;
			std::coroutine_handle<> mCoroHd1{};
		};

		// make the DataStreamReader awaitable
		auto operator co_await() noexcept { return Awaiter{ *this }; }

		void set(std::byte data)
		{
			mData.emplace(data);
			if (mAwaiter)
			{
				mAwaiter->resume();
			}
		}

	private:
		friend struct Awaiter;
		Awaiter* mAwaiter{};
		std::optional<std::byte> mData{};
	};

	using FSM = generator<std::string, false>;

	//static const std::byte ESC{ 'H' };
	//static const std::byte SOF{ 0x10 };


	// the stream is passed by reference into the parse; 
	// need to make sure lifetime of stream is longer than Parse
	FSM Parse(DataStreamReader& stream)
	{
		while (true)
		{
			// used to simulate throwing points
			ThrowIf(Ex::ParseBegin, "ParseBegin");

			// wait for the stream
			std::byte bb = co_await stream;
			std::string frame{};

			if (ESC == bb)
			{
				bb = co_await stream;
				if (SOF != bb)
				{
					continue; // not a start sequence ; this will skip the while loop goback  to outer while loop
				}

				ThrowIf(Ex::ParseWhileRunning, "ParseWhileRunning!");

				// try to capture the full frame
				while (true)
				{
					bb = co_await stream;
					if (ESC == bb)
					{
						// skip this byte look at the next one!
						bb = co_await stream;
						if (SOF == bb)
						{
							// if end of frame then return the result
							co_yield frame;
							break;
						}
						else if (ESC != bb)
						{
							// out of sync !!!
							break;
						}
					}

					frame += static_cast<char>(bb);
				}
			}
		}
	}

	/* std::vector is by copy on purpose because of lifetime issues; as coroutine may live longer that original data
		but the call site can use std::move ; in case you are sure the container lives longer than the coroutine than pass by const&
		; e.g. we are simulation parsing signals on TCP then those might not live longer you pass by copy & move semantics will be applied by compiler
	*/


	// this simulates a network that sends signal until it is disconnected; coroutine send the value of signal into promise_type_base
	generator<std::byte> send(std::vector<std::byte> fakeBytes)
	{
		for (const auto& signal : fakeBytes)
		{
			co_yield signal;
		}
	}

	/*TOTEST: see if string_view to use with printf; probably not since string_view does not have null terminator*/
	//void HandleFrame(std::string_view result)
	void HandleFrame(const std::string& result)
	{
		std::printf("%s\n", result.c_str());
	}

	void PrintException(std::runtime_error& runtimeError)
	{
		std::printf("Exception caught: %s\n", runtimeError.what());
	}


} // end of namespace  PMRMemoryPool



inline void StreamParser_CortnExceptions()
{
	std::printf("\n--StreamParser_CortnExceptions--\n");
	using namespace CortnExcept;
	using namespace std::literals;
	int argc =2;
	std::array argv = {"ParseBegin"s, "ParseWhileRunning"s,"InitialSuspend"s, "YieldValue"s, "GetReturnObject"s };

	if (argc > 2)
	{
		rethrow = std::atoi(argv[1].data());
		
		if ("ParseBegin"s == argv[2])
		{
			defExcept = Ex::ParseBegin;
		}
		else if ("ParseWhileRunning"s == argv[2])
		{
			defExcept = Ex::ParseWhileRunning;
		}
		else if ("InitialSuspend"s == argv[2])
		{
			defExcept = Ex::InitialSuspend;
		}
		else if ("YieldValue"s == argv[2])
		{
			defExcept = Ex::YieldValue;
		}
		else if ("GetReturnObject"s == argv[2])
		{
			defExcept = Ex::GetReturnObject;
		}
	}

	std::vector<std::byte> fakeBytes1{ 0x70_B, 0x24_B, ESC, SOF, ESC,'H'_B, 'e'_B, 'l'_B, 'l'_B, 'o'_B,ESC, SOF, 0x7_B, ESC, SOF };

	// simulate another  network connection
	std::vector<std::byte> fakeBytes2{ 'W'_B, 'o'_B, 'r'_B, 'l'_B, 'd'_B, ESC, SOF, 0x99_B };

	try
	{
		// simulating first network data stream;
		auto stream1 = CortnExcept::send(std::move(fakeBytes1));
		// create a coroutine and store the promise in the coroutine_handle
		DataStreamReader dr{};
		auto p = CortnExcept::Parse(dr);

		for (const auto& data : stream1)
		{
			dr.set(data);

			if (const auto& res = p(); res.length())
			{
				HandleFrame(res);
			}
		}

	/*	auto stream2 = send(std::move(fakeBytes2));
		for (const auto& data : stream2)
		{
			dr.set(data);

			if (const auto& res = p(); res.length())
			{
				HandleFrame(res);
			}
		}*/
	}
	catch(std::runtime_error& rt)
	{
		PrintException(rt);
	}
}