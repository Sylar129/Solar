#pragma once

#include "Solar/Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Solar {
    class SOLAR_API Log {
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
}

// Core log macros
#define SOLAR_CORE_TRACE(...)       ::Solar::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SOLAR_CORE_INFO(...)        ::Solar::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SOLAR_CORE_WARN(...)        ::Solar::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SOLAR_CORE_ERROR(...)       ::Solar::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SOLAR_CORE_CRITICAL(...)    ::Solar::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SOLAR_TRACE(...)            ::Solar::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SOLAR_INFO(...)             ::Solar::Log::GetClientLogger()->info(__VA_ARGS__)
#define SOLAR_WARN(...)             ::Solar::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SOLAR_ERROR(...)            ::Solar::Log::GetClientLogger()->error(__VA_ARGS__)
#define SOLAR_CRITICAL(...)         ::Solar::Log::GetClientLogger()->critical(__VA_ARGS__)
