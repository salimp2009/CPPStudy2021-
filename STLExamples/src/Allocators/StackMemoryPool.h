#pragma once
#include "STLpch.h"

using PtrSize			= std::size_t;
using difference_type	= std::ptrdiff_t;
using U8				= std::uint8_t;
using U32				= std::uint32_t;

class StackMemoryPool
{
public:
	StackMemoryPool(PtrSize size, U32 alignbits = 16);

	StackMemoryPool(const StackMemoryPool&) = delete;
	StackMemoryPool& operator=(const StackMemoryPool&) = delete;

	StackMemoryPool(StackMemoryPool&& other) noexcept
	{
		*this = std::move(other);
	}

	StackMemoryPool& operator=(StackMemoryPool&& other) noexcept;
	
	~StackMemoryPool();

	PtrSize getSize() const noexcept { return memSize; }

	PtrSize getAllocatedSize() const noexcept ;

	void* allocate(PtrSize size) noexcept;

	bool free(void* p) noexcept;

	void reset() noexcept;

private:
	/* pointer to the base of memory*/
	U8* memory{ nullptr };
	
	/*Size of pre-allocated memory chunck*/
	PtrSize memSize{ 0 };
	
	/* pointer to top of the memory stack*/
	std::atomic<U8*>top{ nullptr };

	U32 alignmentBits{ 0 };

	const PtrSize calcAlignSize(PtrSize size) const noexcept;
};