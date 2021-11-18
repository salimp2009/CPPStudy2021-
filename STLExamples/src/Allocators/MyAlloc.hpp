#pragma once
#include "STLpch.h"


template<typename T1, typename T2>
concept ConvertibleType = std::is_convertible_v<T2, T1>;


template<typename T>
class MyAlloc
{

private:

	static inline uint32_t allocCount{ 0 };
public:
	typedef T value_type;
	//constructor
	constexpr MyAlloc() noexcept {}

	/* a template copy constructor does not supress the compiler generated copy constructor
	   if the types are exact match then the compiler generated copy constructor wil be used*/
	template<typename U, typename = std::enable_if<std::is_convertible_v<U, T>>>
	constexpr MyAlloc(const MyAlloc<U>&) noexcept {}

	/*allocate but dont initialize!*/
	[[nodiscard]] constexpr T* allocate(std::size_t num)
	{
		++allocCount;
		fmt::print("Alloc Count: {0}, size: {1}\n", allocCount, num * sizeof(T));
		return static_cast<T*>(::operator new(num * sizeof(T)));
	}

	constexpr void deallocate(T* p, std::size_t num)
	{
		::operator delete(p);
	}

};

template<typename T1, typename T2>
constexpr bool operator==(const MyAlloc<T1>&, const MyAlloc<T2>&) noexcept
{
	return true;
}

template<typename T1, typename T2>
constexpr bool operator!=(const MyAlloc<T1>&, const MyAlloc<T2>&) noexcept
{
	return false;
}



/* E.g; to use custom allocator but better to use with memory-pool; custom or std::pmr */
template<typename T>
using Vec = std::vector<T, MyAlloc<T>>;