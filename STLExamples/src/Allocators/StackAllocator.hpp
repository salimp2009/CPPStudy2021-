#pragma once
#include "Allocators/StackMemoryPool.h"


template<typename T, bool deallocationFlag = false, U32 alignmentBits = 16>
class StackAllocator
{
	template<typename U, bool deallocationflag, U32 alignmentbits>
	friend class StackAllocator;
public:
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	//using pointer				= T*;
	//using const_pointer			= const T*;
	//using reference				= T& // depreciate in c++17; removed in c++20
	//using const_reference		= const T&; // depreciate in c++17; removed in c++20
	using value_type = T;

	using propagate_on_container_move_assignment = std::true_type;


	constexpr StackAllocator() noexcept : mpool{ nullptr } {}

	constexpr StackAllocator(const StackAllocator& b) noexcept { *this = b; }

	template<typename U>
	constexpr StackAllocator(const StackAllocator<U, deallocationFlag, alignmentBits>& b) noexcept
	{
		*this = b;
	}

	constexpr StackAllocator(size_type size) noexcept
	{
		mpool.reset(new StackMemoryPool(size, alignmentBits));
	}

	constexpr ~StackAllocator() noexcept {}

	constexpr StackAllocator& operator=(const StackAllocator& b) noexcept
	{
		mpool = b.mpool;
		return *this;
	}

	template<typename U>
	constexpr StackAllocator& operator=(const StackAllocator<U, deallocationFlag, alignmentBits>& b) noexcept
	{
		mpool = b.mpool;
		return *this;
	}

	[[nodiscard]] constexpr T* allocate(size_type num)
	{
		assert(mpool != nullptr);
		size_type size = num * sizeof(value_type);
		void* out = mpool->allocate(size);
		if (out == nullptr)
		{
			fmt::print("Allocation Failed!: not enough memory!\n");
		}
#if _DEBUG
		std::printf("StackAllocator test: Allocating\n");
#endif //  _DEBUG
		return static_cast<T*>(out);
	}

	constexpr void deallocate(void* p, size_type n)
	{
		assert(mpool != nullptr);
		(void)n;

		if (deallocationFlag)
		{
#if _DEBUG
			std::printf("StackAllocator: Allocating\n");
#endif //  _DEBUG

			auto ok = mpool->free(p);
			if (!ok)
			{
				fmt::print("Freeing wrong pointer!!; DeAllocations should be in order!!\n");
			}
		}

	}

	constexpr size_type maxSize() noexcept
	{
		assert(mpool != nullptr);
		return mpool->getSize();
	}

	constexpr size_type allocatedSize() noexcept
	{
		assert(mpool != nullptr);
		return mpool->getAllocatedSize();
	}

	constexpr void reset() noexcept
	{
		assert(mpool != nullptr);
		mpool->reset();
	}

	const StackMemoryPool& getMemory() const&
	{
		assert(mpool != nullptr);
		return *mpool;
	}


	template<typename U>
	struct rebind
	{
		typedef StackAllocator<U, deallocationFlag, alignmentBits> other;
	};

private:
	std::shared_ptr<StackMemoryPool> mpool{ nullptr };


};


template<typename T1, typename T2, bool deallocationFlag, U32 alignmentBits>
constexpr bool operator ==(const StackAllocator<T1, deallocationFlag, alignmentBits>&, const StackAllocator<T1, deallocationFlag, alignmentBits>&)
{
	return true;
}


template<typename T1, typename AnotherAllocator, bool deallocationFlag, U32 alignmentBits>
constexpr bool operator ==(const StackAllocator<T1, deallocationFlag, alignmentBits>&, const AnotherAllocator&)
{
	return true;
}

template<typename T1, typename T2, bool deallocationFlag, U32 alignmentBits>
constexpr bool operator !=(const StackAllocator<T1, deallocationFlag, alignmentBits>&, const StackAllocator<T1, deallocationFlag, alignmentBits>&)
{
	return false;
}

template<typename T1, typename AnotherAllocator, bool deallocationFlag, U32 alignmentBits>
constexpr bool operator !=(const StackAllocator<T1, deallocationFlag, alignmentBits>&, const AnotherAllocator&)
{
	return false;
}