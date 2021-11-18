#pragma once
#include "RangesPCH.hpp"
#include "Formatting/StockIndex.hpp"

// THIS DOES NOT COMPILE at all!!!!
template<>
struct std::formatter<StockIndex> 
{
    enum class IndexFormat {Normal, Short, WithPlus};

    IndexFormat indexFormat{ IndexFormat::Normal };
    
    constexpr auto parse(std::format_parse_context& ctx)
    {
        auto it = ctx.begin();
        auto end = ctx.end();

        if ((it != end) && (*it == 's'))
        {
            indexFormat = IndexFormat::Short;
            ++it;
        }
        else if ((it!=end) && (*it =='p'))
        {
            indexFormat = IndexFormat::WithPlus;
            ++it;
        }

        // check if at the end of the range
        if (it != end && (*it != '}'))
        {
            throw std::format_error("invalid format!");
        }

        // returning an iterator past the end of the parsed range ??; 
        // TODO: check if this should be end-1
        return it;
    }

    auto format(const StockIndex& index, std::format_context& ctx)
    {
        
        if (IndexFormat::Short == indexFormat)
        {
            return std::format_to(ctx.out(), "{:10}  {:>8.2f}", index.name(), index.points());
        }
        else
        {
            const std::string fmt{ (IndexFormat::WithPlus == indexFormat)  
                                  ? "{:10} {:>8.2f} {:>+7.2f} {:+.2f}%"
                                  : "{:10} {:>8.2f} {:>6.2f} {:.2f}%" };
            
            return std::format_to(ctx.out(), fmt, index.name(), index.points(), index.pointsDiff(), index.pointsPercent());
        }
    }
};

