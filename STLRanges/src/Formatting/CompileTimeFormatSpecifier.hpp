#pragma once
#include "RangesPCH.hpp"
#include "CustomFormatLogger.hpp"

template<std::size_t Args>
constexpr auto makeBraces()
{

	constexpr std::array<char, 4>brace{ "{} " };

	// size of brace without string terminator (\0)
	constexpr auto brace_size = brace.size() - 1;

	// reserve 2 characters for newline and string terminator
	constexpr auto offset{ 2u };

	std::array<char, Args* brace_size + offset> totalBraces{};

	//length of the totalBraces is size of totalBraces - newline and string terminator
	constexpr auto totalBracesLength = (totalBraces.size() - offset);

	auto i{ 0u };
	std::for_each_n(totalBraces.begin(), totalBracesLength, [&] (auto& element)
	{
			element = brace[i % brace_size];
			++i;
	});

	// add the new line to totalBraces array;
	totalBraces[totalBraces.size() - offset] = '\n';

	return totalBraces;
}


template<typename... Args>
void custLoggerV3(LogLevel level, Args&&... args)
{
	constexpr auto fmt = makeBraces<sizeof...(Args)>();
	vlog(level, std::string_view{ fmt.data(), fmt.size() }, std::make_format_args(std::forward<Args>(args)...));
}