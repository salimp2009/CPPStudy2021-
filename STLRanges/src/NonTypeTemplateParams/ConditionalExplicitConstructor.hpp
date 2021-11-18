#include "RangesPCH.hpp"

// used NTTPTests for this one to avoid 

struct MyBool
{
	template<typename T>
	explicit(!std::is_same_v<T, bool>) MyBool(T t)
	{
		fmt::print("{}\n", typeid(t).name());
	}


};

