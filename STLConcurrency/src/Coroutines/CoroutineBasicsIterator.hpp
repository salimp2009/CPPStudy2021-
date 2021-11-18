#pragma once
#include "ConcurrencyPCH.h"


class IntRange
{
public:
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;

	//template<typename PT>
	//struct iteratorType;
	//using iterator = iteratorType<promise_type>;

	IntRange(handle_type h) :coroHandle{ h } {}
	~IntRange() { if (coroHandle) coroHandle.destroy(); }

	// no copying but move is OK
	IntRange(const IntRange&) = delete;
	IntRange(IntRange&& rhs) noexcept : coroHandle{ std::exchange(rhs.coroHandle, nullptr) } {}
	IntRange& operator=(const IntRange&) = delete;

	struct promise_type
	{
		int currentValue;

		auto get_return_object() { return handle_type::from_promise(*this); }
		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }

		auto yield_value(int value)
		{
			currentValue = value;
			return std::suspend_always{};
		}

		void return_void() {}
		void unhandled_exception() { std::terminate(); }

	};

	struct iterator
	{
		handle_type handle;

		iterator(handle_type p): handle{p} {}

		// this approach seems over kill (TODO:check with other librarires) & see CoroutineByteStreamParser examples for a different iterator approach !
		void getNext()
		{
			if (handle)
			{
				handle.resume();
				if (handle.done())
				{
					handle = nullptr;
				}
			}
		}

		int operator*() const
		{
			assert(handle != nullptr);
			return handle.promise().currentValue;
		}

		iterator operator++()
		{
			getNext();
			return *this;
		}

		bool operator==(const iterator&) const = default;

	};

	iterator begin() const
	{
		if (!coroHandle || coroHandle.done())
		{
			return iterator{ nullptr };
		}

		// initialize iterator
		iterator itor{ coroHandle };
		// resume for first value
		itor.getNext();
		return itor;
	}

	iterator end() const
	{
		return iterator{ nullptr };
	}

private:
	handle_type coroHandle;
};

template<typename T>
inline IntRange loopOverRange(const T& coll)
{
	for (int value : coll)
	{
		std::puts("- suspend");
		co_yield value;
		std::puts("- resume");
	}
}