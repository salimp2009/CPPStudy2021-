#pragma once
#include "Templpch.h"


template<typename T>
union storage_t
{
	using aligned_storage_t = std::aligned_storage_t<sizeof(T), alignof(T)>;
	aligned_storage_t data;
	
	storage_t() = default;
	
	//T* as() { return reinterpret_cast<T*>(&data); }
	// use placement new to create an instance of T inside this union ??
	/*TODO: CHECK if this works OK */
	T* as() { return reinterpret_cast<T*>(new(&data)T()); }
};

struct  copyable {};

struct notCopyable
{
	//notCopyable() = default;
	notCopyable(const notCopyable&) = delete;
	notCopyable& operator=(const notCopyable) = delete;
};

struct Not_TriviallyDestructible
{
	~Not_TriviallyDestructible() {};
};


struct COMLike
{
	~COMLike() = delete; // not default destructible
	void Release() { std::printf("COMLike releasing all data!\n"); }
};

template<typename T>
concept HasRelease = requires(T t)
{
	t.Release();
};

template<typename T>
concept NotTriviallyDestructible = ! std::is_trivially_destructible_v<T>;


template<typename T>
class optional
{
public:
	optional() = default;

	optional(const optional& other) requires std::is_copy_constructible_v<T> = default;

	//template<typename U, typename =std::enable_if_t<std::is_same_v<U, optional> && std::is_copy_constructible_v<T>>>
	//optional(const U&) {};


	/* the ordering of concepts is from strong to weak constraint*/
	~optional() requires (NotTriviallyDestructible<T>)
	{  
		if (has_value)
		{
			value.as()->~T();
		}
		std::printf("optional destructor: NotTriviallyDestructible<T>\n");
	}

	~optional() requires (NotTriviallyDestructible<T>&& HasRelease<T>)
	{
		if (has_value)
		{
			value.as()->Release();
			value.as()->~T();
		}
		std::printf("optional destructor: NotTriviallyDestructible<T>&& HasRelease<T>\n");
	}

	~optional() requires  HasRelease<T>
	{
		if (has_value)
		{
			value.as()->Release();
		}
		std::printf("optional: HasRelease<T>\n");
	}

	// this does not compile in Clang 12; OK with GCC 11; it is a cLang bug
	~optional() = default;

private:
	storage_t<T> value{};
	bool has_value{ true };
};

optional<notCopyable>a{};
//optional<notCopyable>b = a; // Will fail since it is not copy_constructible

static_assert(not std::is_copy_constructible_v<optional<notCopyable>>);
static_assert(std::is_copy_constructible_v<optional<int>>);

static_assert(not std::is_trivially_destructible_v<optional<Not_TriviallyDestructible>>);
static_assert(std::is_trivially_destructible_v<optional<notCopyable>>);
static_assert(std::is_trivially_destructible_v<optional<int>>);


