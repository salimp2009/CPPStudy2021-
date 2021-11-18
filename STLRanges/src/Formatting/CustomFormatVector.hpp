#pragma once
#include "RangesPCH.hpp"

// THIS DOES NOT COMPILE with std::format but works with fmt::format
template<typename T>
struct std::formatter<std::vector<T>>
{
	std::string formatString;

	auto constexpr parse(std::format_parse_context& ctx)
	{
		formatString = "{:";
		std::string parseContext(std::begin(ctx), std::end(ctx));
		formatString += parseContext;
		return std::end(ctx) - 1;
	}

	template<typename FormatContext>
	auto format(const std::vector<T>& v, FormatContext& ctx)
	{
		auto out = ctx.out();
		std::format_to(out, "[");
		
		if (v.size() > 0) std::format_to(out, formatString, v[0]);

		for (int i = 1; i < v.size(); ++i)
		{
			std::format_to(out, ", " + formatString, v[i]);
		}
		std::format_to(out, "]");
		return std::format_to(out, "\n");
	}
};

// EXAMPLE for Nico's c++20 book works
enum class Color { red, green, blue };
template<>
struct std::formatter<Color> : std::formatter<std::string_view> {
	auto format(Color c, format_context& ctx) {
		switch (c) {
		case Color::red:
			return std::formatter<std::string_view>::format("red {}", ctx);
		case Color::green:
			return std::formatter<std::string_view>::format("green", ctx);
		case Color::blue:
			return std::formatter<std::string_view>::format("blue", ctx);
		}
		return std::formatter<std::string_view>::format("oops", ctx);
	}
};


