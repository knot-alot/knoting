#pragma once

#include <knoting/log.h>

#if defined(KNOTING_DEBUG)

#if defined(__linux__) || defined(__APPLE__)
#include <signal.h>
#define KNOTING_DEBUG_BREAK() raise(SIGTRAP)
#elif defined(_WIN32)
#define KNOTING_DEBUG_BREAK() __debugbreak()
#endif  // OS

#if __has_include("spdlog/spdlog.h")
#define KNOTING_LOG_ERROR(...) spdlog::error(__VA_ARGS__);
#define KNOTING_LOG_CURRENT_POSITION()                                 \
    KNOTING_LOG_ERROR("Assertion triggered at:\n\t{}:{}:{}", __FILE__, \
                      __FUNCTION__, __LINE__);
#else
#include <iostream>
inline void KNOTING_LOG_ERROR() {}
template <typename First, typename... Rest>
inline void KNOTING_LOG_ERROR(First&& first, Rest&&... rest) {
    std::cerr << std::forward<First>(first);
    KNOTING_LOG_ERROR(std::forward<Rest>(rest)...);
}

#define KNOTING_LOG_CURRENT_POSITION()                              \
    KNOTING_LOG_ERROR("Assertion triggered at:\n\t", __FILE__, ":", \
                      __FUNCTION__, ":", __LINE__, "\n");
#endif  // __has_include("spdlog/spdlog.h")

#define KNOTING_ASSERT_MESSAGE(condition, ...) \
    {                                          \
        if (!(condition)) {                    \
            KNOTING_LOG_CURRENT_POSITION();    \
            KNOTING_LOG_ERROR(__VA_ARGS__);    \
            KNOTING_DEBUG_BREAK();             \
        }                                      \
    }

#define KNOTING_ASSERT(condition)           \
    {                                       \
        if (!(condition)) {                 \
            KNOTING_LOG_CURRENT_POSITION(); \
            KNOTING_DEBUG_BREAK();          \
        }                                   \
    }

#else

#define KNOTING_ASSERT_MESSAGE(contition, ...)
#define KNOTING_ASSERT(condition)

#endif  // KNOTING_DEBUG
