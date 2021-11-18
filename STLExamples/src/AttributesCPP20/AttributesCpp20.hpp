#pragma once
#include "STLpch.h"

struct Mytype
{
	[[nodiscard("Implicitly destroying of temp MyInt")]] Mytype(int, bool) {}
};

template <typename T, typename... Args>
[[nodiscard("Memory Leak:Dont Discard!!!")]] T* create(Args&&... args)
{
	return new T(std::forward<Args>(args)...);
}

enum class [[nodiscard("Dont Ignore the Error!!!")]] ErrorCode
{
	Okay,
	Warning,
	Critical,
	Fatal
};

ErrorCode errorProneFunction() { return ErrorCode::Fatal; }