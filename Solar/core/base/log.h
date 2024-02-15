// Copyright (c) 2024 Sylar129

#pragma once

#include <memory>

#include "spdlog/logger.h"

namespace solar {

class Log {
 public:
  static void Init();

  inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
    return s_CoreLogger;
  }

  inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
    return s_ClientLogger;
  }

 private:
  static std::shared_ptr<spdlog::logger> s_CoreLogger;
  static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

}  // namespace solar

// Core log macros
#define SOLAR_CORE_TRACE(...) ::solar::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SOLAR_CORE_INFO(...) ::solar::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SOLAR_CORE_WARN(...) ::solar::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SOLAR_CORE_ERROR(...) ::solar::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SOLAR_CORE_CRITICAL(...) \
  ::solar::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SOLAR_TRACE(...) ::solar::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SOLAR_INFO(...) ::solar::Log::GetClientLogger()->info(__VA_ARGS__)
#define SOLAR_WARN(...) ::solar::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SOLAR_ERROR(...) ::solar::Log::GetClientLogger()->error(__VA_ARGS__)
#define SOLAR_CRITICAL(...) \
  ::solar::Log::GetClientLogger()->critical(__VA_ARGS__)
