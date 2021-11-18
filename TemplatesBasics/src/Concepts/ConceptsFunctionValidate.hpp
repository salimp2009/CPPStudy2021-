#pragma once
#include "Templpch.h"

template<typename T>
concept SupportsValidation = requires(T t)
{
	t.validate();
};

namespace ValidatedCheck
{
	template<typename T>
	void Send(const T& data)
	{
		if constexpr (SupportsValidation<T>)
		{
			data.validate();
		}

		std::printf("Sending data!\n");
	}

} // end of namespace ValidatedCheck

class SimpleType {};

class ComplexType
{
public:
	void validate() const noexcept { std::printf("Data validated!\n"); }
};

