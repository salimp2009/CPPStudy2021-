#pragma once

#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <memory>
#include <utility>
#include <typeinfo>
#include <tuple>
#include <functional>
#include <ratio>
#include <chrono>
#include <numeric>
#include <variant>
#include <optional>
#include <memory_resource>

#if __has_include(<ranges>)
	#include <ranges>
#endif

#if __has_include(<concepts>)
	#include <concepts>
#endif

#include <thread>
#include <future>
#include <execution>

#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"
#include "spdlog/formatter.h"
#include "spdlog/fmt/bundled/ranges.h"
#include "spdlog/fmt/bundled/color.h"


#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <set>
#include <deque>

