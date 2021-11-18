#pragma once
#include "STLpch.h"

/* to enable go PMRMemoryResources.h and change the value TRACKON 1*/
#if TRACKON

struct AllocationMetrics
{
	static inline int allocCount{ 0 };
	static inline std::size_t  allocSize{ 0 };

    static void reset() noexcept
    {
        allocCount = 0;
        allocSize = 0;
    }

    static void printAllocMetrics() noexcept
    {
        std::printf("%d allocations and %zu bytes \n", allocCount, allocSize);
    }


};

static AllocationMetrics allocmetrics;

[[nodiscard]] void* operator new (std::size_t size)
{
    allocmetrics.allocCount++;
    allocmetrics.allocSize += size;
    return ::malloc(size);
}

[[nodiscard]] void* operator new (std::size_t size, std::size_t align)
{
    allocmetrics.allocCount++;
    allocmetrics.allocSize += size;
#ifdef _MSC_VER
    return ::_aligned_malloc(size, align);     // Windows API
#else
    return std::aligned_alloc(align, size);    // C++17 API
#endif
  
}


[[nodiscard]] void* operator new[](std::size_t size)
{
    allocmetrics.allocCount++;
    allocmetrics.allocSize += size;
    return ::malloc(size);
}

[[nodiscard]] void* operator new [](std::size_t size, std::size_t align)
{
    allocmetrics.allocCount++;
    allocmetrics.allocSize += size;
#ifdef _MSC_VER
    return ::_aligned_malloc(size, align);     // Windows API
#else
    return std::aligned_alloc(align, size);    // C++17 API
#endif

}


void operator delete (void* p) noexcept 
{
    std::free(p);
}

void operator delete (void* p, std::align_val_t) noexcept {
#ifdef _MSC_VER
    _aligned_free(p);  // Windows API
#else
    std::free(p);      // C++17 API
#endif
}

#endif // TRACKON macro to disable/enable global overloads