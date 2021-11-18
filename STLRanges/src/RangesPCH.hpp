#pragma once

#include <iostream>
#include <cstdio>
#include <type_traits>
#include <algorithm>
#include <memory>
#include <utility>
#include <typeinfo>
#include <tuple>
#include <functional>
#include <ratio>
#include <chrono>
#include <random>
#include <exception>
#include <ranges>

#include <numeric>
#include <variant>
#include <numbers>
#include <bitset>

#if __has_include(<span>)
#include <span>
#endif

#if __has_include(<ranges>)
#include <ranges>
#endif

#if __has_include(<concepts>)
#include <concepts>
#endif

#if __has_include(<coroutine>)
#include <coroutine>
#endif

#if __has_include(<format>)
#include <format>
#endif

#if __has_include(<source_location>)
#include <source_location>
#endif

#include <thread>
#include <future>
#include <execution>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <csignal>
#include <shared_mutex>
#include <execution>

#if __has_include(<semaphore>)
#include <semaphore>
#endif

#if __has_include(<latch>)
#include <latch>
#endif

#if __has_include(<barrier>)
#include <barrier>
#endif

#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/fmt/bundled/ranges.h"
#include "spdlog/fmt/bundled/color.h"

#include "plf_colony/plf_colony.h"

#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <set>
#include <deque>