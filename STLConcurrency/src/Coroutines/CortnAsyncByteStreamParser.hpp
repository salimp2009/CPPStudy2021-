#pragma once
#include "ConcurrencyPCH.h"

// overloading literals; user defined literals https://en.cppreference.com/w/cpp/language/user_literal
std::byte operator""_B(char c)
{
	return static_cast<std::byte>(c);
}

std::byte operator""_B(unsigned long long  c)
{
	return static_cast<std::byte>(c);
}


namespace AsyncParse
{
	template<typename T, typename G, typename... Bases>
	struct promise_type_base: public Bases...
	{
		/* value yielded from a coroutine*/
		/* mValue is the value that will be returned to the caller by yield_value*/
		T mValue;

		/* invoked by co_yield or co_return*/
		auto yield_value(T value)
		{
			mValue = value;
			return std::suspend_always{};
		}

		/*return the generator*/
		G get_return_object() { return G{ this }; }

		std::suspend_always initial_suspend()		 { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void				return_void() {}
		void				unhandled_exception() { std::terminate(); }
	};

	namespace coro_iterator
	{
		template<typename PT>
		struct iterator
		{
			using coro_handle = std::coroutine_handle<PT>;

			coro_handle mCoroHd1{ nullptr };
			bool		mDone{ true };

			using RetType = decltype(mCoroHd1.promise().mValue);

			void resume()
			{
				mCoroHd1.resume();
				mDone = mCoroHd1.done();
			}

			iterator() = default;

			iterator(coro_handle hco) : mCoroHd1{ hco } { resume(); }

			iterator& operator++()
			{
				resume();
				return *this;
			}

			bool operator==(const iterator& other) const { return mDone == other.mDone; }

			const RetType& operator*()  const& { return mCoroHd1.promise().mValue; }
			const RetType* operator->() const { return &(operator*()); }
		};

	} // end of namespace coro_iterator



	template<typename T>
	struct awaitable_promise_type_base
	{
		std::optional<T> mRecentSignal;

		struct awaiter
		{
			/* mRecentSignal is the std::byte we are parsing; if it matches certain pattern than 
			   the value will be stored in mValue in base promise type; Once its finished the mValue will be returned to caller */
			std::optional<T>& mRecentSignal;

			bool await_ready() { return mRecentSignal.has_value(); }
			void await_suspend(std::coroutine_handle<>) { }

			T await_resume()
			{
				assert(mRecentSignal.has_value());
				auto tmp = *mRecentSignal;
				mRecentSignal.reset();
				return tmp;
			}
		};

		[[nodiscard]] awaiter await_transform(T) { return awaiter{ mRecentSignal }; }
	};

	// Concept to make sure the class has a member function clear() since we use in async_generator
	template<typename T>
	concept HasClear = requires(T obj)
	{
		obj.clear();
	};


	template<HasClear T, typename U>
	struct [[nodiscard]] async_generator
	{
		using promise_type = promise_type_base<T, async_generator, awaitable_promise_type_base<U> >;
		using PromiseTypeHandle = std::coroutine_handle<promise_type>;

		T operator()()
		{
			// move clears the value of promise but leave it undefined state
			auto tmp{ std::move(mCoroHd1.promise().mValue)};
			// set the promise to a defined state ; clear() is std::string member function used HasClear concept to make sure it exists !
			mCoroHd1.promise().mValue.clear();

			return tmp;
		}

		void SendSignal(U signal)
		{
			mCoroHd1.promise().mRecentSignal = signal;
			if (not mCoroHd1.done()) { mCoroHd1.resume(); }
		}

		async_generator(const async_generator&) = delete;
		async_generator(async_generator&& rhs) : mCoroHd1{ std::exchange(rhs.mCoroHd1, nullptr) } {}

		~async_generator() { if (mCoroHd1) mCoroHd1.destroy(); }

	private:
		friend promise_type;
		explicit async_generator(promise_type* p) : mCoroHd1(PromiseTypeHandle::from_promise(*p)) {}

		PromiseTypeHandle mCoroHd1;
	};

	template<typename T>
	struct generator
	{
		using promise_type		= promise_type_base<T, generator>;
		using PromiseTypeHandle = std::coroutine_handle<promise_type>;
		using iterator			= coro_iterator::iterator<promise_type>;

		iterator begin() { return mCoroHd1; }
		iterator end()	 { return {}; }

		generator(const generator&) = delete;
		generator(generator&& rhs) : mCoroHd1{rhs.mCoroHd1} { rhs.mCoroHd1 = nullptr; }

		~generator() { if (mCoroHd1) mCoroHd1.destroy(); }

	private:
		friend promise_type;
		explicit generator(promise_type* p) : mCoroHd1{PromiseTypeHandle::from_promise(*p)} {}
		PromiseTypeHandle mCoroHd1;
	};

	using FSM = async_generator<std::string, std::byte>;

	static  const std::byte ESC{ 'H' };
	static const std::byte SOF{ 0x10 };

	// this coroutine produces async_generator therefore uses async_promise_base which is also awaiter
	FSM Parse()
	{
		while (true)
		{
			std::byte bb = co_await std::byte{};

			std::string frame{};

			if (ESC == bb)
			{
				bb = co_await std::byte{};
				if (SOF != bb)
				{
					continue; // not a start sequence 
				}

				while (true)  // try to capture the full frame
				{
					bb = co_await std::byte{};
					if (ESC == bb)
					{
						// skip this byte look at the next one!
						bb = co_await std::byte{};
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
		//std::printf("%s\n", result.data());
	}

	void ProcessStream(generator<std::byte>& stream, FSM& parse)
	{
		for (const auto& data : stream)
		{
			// sending new byte to waiting Parse
			parse.SendSignal(data);

			// check if the frame is complete and send it to print or do other stuff with it
			if (const auto& res = parse(); res.length())
			{
				HandleFrame(res);
			}
		}
		
	}

} // end of namespace AsyncParse


inline void AsyncByteStreamParser_Cortn()
{
	std::printf("\n--AsyncByteStreamParser--\n");
	std::vector<std::byte> fakeBytes1{
	0x70_B, 0x24_B, ESC, SOF, ESC,
	'H'_B, 'e'_B, 'l'_B, 'l'_B, 'o'_B,
	ESC, SOF, 0x7_B, ESC, SOF };

	// simulating first network data stream;
	auto stream1 = AsyncParse::send(std::move(fakeBytes1));
	// create a coroutine and store the promise in the coroutine_handle
	auto p = AsyncParse::Parse();
	// Process the bytes
	ProcessStream(stream1, p);

	 //simulate another  network connection
	std::vector<std::byte> fakeBytes2{
	'W'_B, 'o'_B, 'r'_B, 'l'_B, 'd'_B, ESC, SOF, 0x99_B };

	auto stream2 = AsyncParse::send(std::move(fakeBytes2));
	// create a coroutine and store the promise in the coroutine_handle
	// Process the bytes
	ProcessStream(stream2, p);
}