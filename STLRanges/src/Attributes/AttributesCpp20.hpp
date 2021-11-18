#pragma once
#include "RangesPCH.hpp"

struct  Mytype
{
	[[nodiscard("Implicitly ignoring!!!!")]] Mytype(int, bool) {}
};

template <typename T, typename... Args>
[[nodiscard("Dont Disgard; Memory Leak!!!")]] T* create(Args&&... args)
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

struct Empty {};

struct NoUniqueAddress
{
	int d{};
	[[no_unique_address]] Empty e;
};

struct UniqueAddress
{
	int d{};
	Empty e;
};