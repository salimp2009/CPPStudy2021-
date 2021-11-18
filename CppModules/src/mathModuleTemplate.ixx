module;

#include <typeinfo>
#include <utility>


export module mathTemp;


template<typename T>
auto showType(T&& t)
{
	return typeid(std::forward<T>(t)).name();
}

export namespace tmath
{
	template<typename T, typename T2>
	auto sum(T first, T2 second)
	{
		return first + second;
	}

	template<typename T, typename T2>
	auto sum2(T first, T2 second)
	{
		auto result= first + second;
		return std::make_pair(result, showType(result));
	}

}