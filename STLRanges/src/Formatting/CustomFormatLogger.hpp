#pragma once
#include "RangesPCH.hpp"


enum LogLevel {Info, Warning, Error};

template<>
struct std::formatter<LogLevel> :std::formatter<std::string_view>
{
	static constexpr const char* LEVEL_NAMES[] = {"Info", "Warning", "Error"};

	auto format(LogLevel c, std::format_context& ctx)
	{
		return std::formatter<std::string_view>::format(LEVEL_NAMES[(c)], ctx);
	}
};

// TODO: make a revised version of this to used std::format_args and std::make_format_args and std::vformat for Args... //
// othervise std::format will create a seperatea new log function will be called causing a larger binary
// std::vformat is non-template version of std::format
template<typename... Args>
void custLoggger(LogLevel level, std::string_view fmt, Args&&... args)
{
	fmt::print("{} {} \n", std::format("{}!!", level), std::format(fmt, std::forward<Args>(args)...));
}


auto GetTime()
{
	auto sysNow = std::chrono::system_clock::now();
	return std::chrono::current_zone()->to_local(sysNow);
}


void vlog(LogLevel level, std::string_view fmt, std::format_args&& args )
{
	auto timeNow = GetTime();
	
	// Original example used C-Api /POSIX date for localtime
	//fmt::print("{2:}, {0}: {1}\n", std::format("{}!!:", level), std::vformat(fmt, args), std::format("[{:%Y-%m-%d-%H:%M:%S}]", timeNow) );

	// the current calendar format for std::chrono / timenow is same above by default!
	fmt::print("{2:}, {0}: {1}\n", std::format("{}!!:", level), std::vformat(fmt, args), std::format("[{}]", timeNow) );
}

template<typename... Args>
void custLoggerV2(LogLevel level, std::string_view fmt, Args&&... args)
{
	vlog(level, fmt, std::make_format_args(std::forward<Args>(args)...));
}