#pragma once
#include "RangesPCH.hpp"
#include "Formatting/StockIndex.hpp"

// THIS DOES NOT COMPILE at all!!!!
template<>
struct std::formatter<StockIndex> {
    enum class IndexFormat { Normal, Short, WithPlus };

    IndexFormat indexFormat{ IndexFormat::Normal };

    bool localized =
        false;  // #A New member to track whether the formatting is localized

    constexpr auto parse(format_parse_context& ctx)
    {
        auto it = ctx.begin();

        auto isChar = [&](char c) {  // #B Helper to search for a character
            if ((it != ctx.end()) && (*it == c)) {
                ++it;
                return true;
            }

            return false;
        };

        if (isChar('L')) { localized = true; }  // #C Localized formatting

        if (isChar('s')) {
            indexFormat = IndexFormat::Short;
        }
        else if (isChar('p')) {
            indexFormat = IndexFormat::WithPlus;
        }

        if (it != ctx.end() && *it != '}') { throw format_error("invalid format"); }

        return it;
    }

    auto format(const StockIndex& index, format_context& ctx)
    {
        const std::string locFloat{ localized ? "L" : "" };  // #D Add localized
        const std::string plus{ (IndexFormat::WithPlus == indexFormat) ? "+" : "" };

        if (IndexFormat::Short == indexFormat) {
            const auto fmt = std::format("{{:10}} {{:>8.2{}f}}", locFloat);
            return std::format_to(ctx.out(), fmt, index.name(), index.points());

        }
        else {
            const auto fmt{
              std::format("{{:10}} {{:>8.2{0}f}}  {{:>{1}7.2{0}f}}  {{:{1}.2{0}f}}%",
                          locFloat,
                          plus) };

            return std::format_to(ctx.out(),
                fmt,
                index.name(),
                index.points(),
                index.pointsDiff(),
                index.pointsPercent());
        }
    }
};
