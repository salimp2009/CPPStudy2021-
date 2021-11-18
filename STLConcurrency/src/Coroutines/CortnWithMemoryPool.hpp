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


namespace ParseMemoryPool
{
	struct arena
	{
		void* Allocate(std::size_t size) noexcept;
		void DeAllocate(void* p, std::size_t size) noexcept;

		static arena* GetFromPtr(void* ptr, std::size_t size);
	};

	void* arena::Allocate(std::size_t size) noexcept
	{
		auto objectsize = size;
		/* add the size of arena pointer so we can get the address of ptr
		   original size will be for data and added size for arena* ptr to be able to pass arnd */
		size += sizeof(arena*);
		
		// allocate memory
		char* ptr = new char[size];

		/* return the address of pointer ; Check if std::bit_cast can be used  https://en.cppreference.com/w/cpp/numeric/bit_cast */
		[[maybe_unused]] arena* aa = reinterpret_cast<arena*>(ptr + objectsize);
		
		/* every invocation will get a seperate arena/memory location*/
		aa = this;
		
		/* Not sure why this is needed ; for comparision of a & bthis is original location of pointer ; not the */
		arena* bb = reinterpret_cast<arena*>(ptr + objectsize);

		std::printf("custom allocate: object size: %zu, ptr: %p, address of instance: %p, address of the pointer(ptr+objectsize): %p\n", objectsize, ptr, this, bb);

		return ptr;
	}

	arena* arena::GetFromPtr(void* ptr, std::size_t size)
	{
		// this is to access the objects memory ptr and then call the right memory location to delete; used with Delete
		std::printf(" ptr: %p,  address of the pointer(ptr+objectsize): %p\n", ptr, (static_cast<char*>(ptr) + size));
		return reinterpret_cast<arena*>(static_cast<char*>(ptr) + size);
	}

	void arena::DeAllocate(void* ptr, std::size_t size) noexcept
	{
		std::printf("custom deallocation: size: %zu, from ptr: %p, this: %p\n", size, ptr, this);
		return delete[] static_cast<char*>(ptr);
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
			mValue = value;
			return {};
		}

		auto initial_suspend()
		{
			if constexpr (InitialSuspend)
			{
				//std::printf("promise:initial_suspend!\n");
				return std::suspend_always{};
			}
			else
			{
				return std::suspend_never{};
			}
		}

		std::suspend_always final_suspend() noexcept { return {}; }
		/*return the generator*/
		G get_return_object()
		{
			//std::printf("promise:return Object!\n");
			return G{ this };
		}
		void				return_void() {}
		void				unhandled_exception() { std::terminate(); }

		
		template<typename... TheRest>
		void* operator new(std::size_t size, arena& a, TheRest&&...) noexcept
		{
			std::printf("promise:call Arena Allocate!\n");
			return a.Allocate(size);
		}
		void operator delete(void* ptr, std::size_t size) noexcept
		{
			std::printf("promise:call DeAllocate!\n");
			arena::GetFromPtr(ptr, size)->DeAllocate(ptr, size);
	
		}

		/* this will allow if operator new to be noexcept ; if new fails then an object type G with nullptr will returned so it no throwing !*/
		static auto get_return_object_on_allocation_failure() { return G{ nullptr }; }
	};

	namespace coro_iterator
	{
		template<typename PT>
		struct iterator
		{
			using coro_handle = std::coroutine_handle<PT>;

			coro_handle mCoroHd1{};

			using RetType = decltype(mCoroHd1.promise().mValue);

			void resume(){mCoroHd1.resume();}

			iterator() = default;

			iterator(coro_handle hco) : mCoroHd1{ hco } { resume(); }

			void operator++() { resume(); }

			bool operator==(const iterator& ) const { return mCoroHd1.done(); }
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
		iterator end() { return {}; }

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

		// disabling the move assignment operator ; this also disables the default copy operations
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

	static const std::byte ESC{ 'H' };
	static const std::byte SOF{ 0x10 };


	// the stream is passed by reference into the parse; 
	// need to make sure lifetime of stream is longer than Parse
	FSM Parse(arena& ar, DataStreamReader& stream)
	{
		while (true)
		{
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
	generator<std::byte> send(arena& ar, std::vector<std::byte> fakeBytes)
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
	

} // end of namespace AsyncParse2



inline void StreamParser_MemoryPool()
{
	std::printf("\n--StreamParser_MemoryPool--\n");
	std::vector<std::byte> fakeBytes1{ 0x70_B, 0x24_B, ESC, SOF, ESC,'H'_B, 'e'_B, 'l'_B, 'l'_B, 'o'_B,ESC, SOF, 0x7_B, ESC, SOF };

	// simulate another  network connection
	std::vector<std::byte> fakeBytes2{ 'W'_B, 'o'_B, 'r'_B, 'l'_B, 'd'_B, ESC, SOF, 0x99_B };

	ParseMemoryPool::arena a1{};
	ParseMemoryPool::arena a2{};
	// simulating first network data stream;
	auto stream1 = ParseMemoryPool::send(a1, std::move(fakeBytes1));
	// create a coroutine and store the promise in the coroutine_handle
	ParseMemoryPool::DataStreamReader dr{};
	auto p = ParseMemoryPool::Parse(a2, dr);

	for (const auto& data : stream1)
	{
		dr.set(data);

		if (const auto& res = p(); res.length())
		{
			ParseMemoryPool::HandleFrame(res);
		}
	}



	auto stream2 = ParseMemoryPool::send(a1, std::move(fakeBytes2));
	for (const auto& data : stream2)
	{
		dr.set(data);

		if (const auto& res = p(); res.length())
		{
			ParseMemoryPool::HandleFrame(res);
		}
	}
}
