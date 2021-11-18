#pragma once
#include <iostream>
#include <limits>
#include <bitset>
#include <cstdint>
#include <cassert>
#include <cstddef>

#include <thread>
#include <future>
#include <execution>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <csignal>

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
#include "spdlog/spdlog.h"
