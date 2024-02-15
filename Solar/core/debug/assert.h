// Copyright (c) 2024 Sylar129

#pragma once

#include "core/base/log.h"

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
