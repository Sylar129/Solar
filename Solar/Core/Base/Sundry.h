// Copyright (c) 2024 Sylar129

#pragma once

#include <memory>
#include <utility>

#ifdef SOLAR_DEBUG
#ifdef SOLAR_PLATFORM_WINDOWS
#define SOLAR_DEBUGBREAK() __debugbreak()
#elif defined SOLAR_PLATFORM_LINUX
#include <signal.h>
#define SOLAR_DEBUGBREAK() raise(SIGTRAP)
#endif
#define SOLAR_ENABLE_ASSERT
#else
#define SOLAR_DEBUGBREAK()
#endif  // SOLAR_DEBUG

#ifdef SOLAR_ENABLE_ASSERT
#define SOLAR_ASSERT(x, ...)                             \
  {                                                      \
    if (!(x)) {                                          \
      SOLAR_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
      SOLAR_DEBUGBREAK();                                \
    }                                                    \
  }
#define SOLAR_CORE_ASSERT(x, ...)                             \
  {                                                           \
    if (!(x)) {                                               \
      SOLAR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
      SOLAR_DEBUGBREAK();                                     \
    }                                                         \
  }
#else
#define SOLAR_ASSERT(x, ...)
#define SOLAR_CORE_ASSERT(x, ...)
#endif

#define SOLAR_BIND_EVENT_FN(fn)                             \
  [this](auto&&... args) -> decltype(auto) {                \
    return this->fn(std::forward<decltype(args)>(args)...); \
  }

namespace solar {

template <bool SHIFT = true>
constexpr auto BIT(int n) {
  if constexpr (SHIFT) {
    return 1 << n;
  } else {
    return n;
  }
}

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

}  // namespace solar
