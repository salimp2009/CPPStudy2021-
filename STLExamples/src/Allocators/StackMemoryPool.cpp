#include "STLpch.h"
#include "Allocators/StackMemoryPool.h"


struct MemoryBlockHeader
{
	U32 size{ 0 };
};

StackMemoryPool::StackMemoryPool(PtrSize size, U32 alignbits):memSize{size}, alignmentBits{alignbits}
{
	assert(size > 0);
	memory = static_cast<U8*>(::_aligned_malloc(memSize, alignmentBits));

	if (memory != nullptr)
	{
		top.store(memory);
	}
	else
	{
		fmt::print("aligned_malloc() failed!");
	}

}

StackMemoryPool::~StackMemoryPool()
{
	if (memory != nullptr)
	{
		::_aligned_free(memory);
		
	}
}


StackMemoryPool& StackMemoryPool::operator=(StackMemoryPool&& other) noexcept
{
	if(memory!=nullptr)
	{
		::_aligned_free(memory);
	}

	memory = other.memory;
	memSize = other.memSize;
	top.store(other.top.load());

	other.memory = nullptr;
	other.memSize = 0;
	other.top=nullptr;

	return *this;
}


PtrSize StackMemoryPool::getAllocatedSize() const noexcept
{
	return top.load()-memory;
}

void* StackMemoryPool::allocate(PtrSize size) noexcept
{
	assert(memory != nullptr);
	size = calcAlignSize(size + sizeof(MemoryBlockHeader));

	/*check if the size is bigger to fit in 32 bits*/
	assert(size < std::numeric_limits<U32>::max());

	U8* out = top.fetch_add(size);

	if (out + size <= memory + memSize)
	{
		((MemoryBlockHeader*)out)->size = static_cast<U32>(size);

		out += sizeof(MemoryBlockHeader);
	}
	else
	{
		/*error!*/
		out = nullptr;
		fmt::print("Not enough memory size!");
	}
#if  _DEBUG
	std::printf("Memory pool: Allocating\n");
#endif //  _DEBUG

	
	return static_cast<void*>(out);
}

bool StackMemoryPool::free(void* p) noexcept
{
	/* Correct the p for the added memory block header*/
	U8* realptr = static_cast<U8*>(p) - sizeof(MemoryBlockHeader);
	
	/* check realptr is within the pools preallocated memory*/
	assert(realptr >= memory && realptr < memory + memSize);

	U32 size = ((MemoryBlockHeader*)realptr)->size;

	/* the top of the stack has to be revised to free memory from the stack via atomic operations
		TODO : check if desired should be const so it can be adjusted by another thread
	*/
	U8* expected = realptr + size;
	U8* desired = realptr;
	auto exchange = top.compare_exchange_strong(expected, desired);
		
	return exchange;
}

void StackMemoryPool::reset() noexcept
{
	/*memory might be nullptr if moved*/
	assert(memory != nullptr);

	/* revised this from top = memory to make it safe for multithread*/
	top.store(memory);
	
}

const PtrSize StackMemoryPool::calcAlignSize(PtrSize size) const noexcept
{
	return size + (size%(alignmentBits/8));
}
