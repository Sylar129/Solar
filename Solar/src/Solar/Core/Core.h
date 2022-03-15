#pragma once

#include <memory>

#ifdef SOLAR_PLATFORM_WINDOWS
    #if SOLAR_DYNAMIC_LINK
        #ifdef SOLAR_BUILD_DLL
            #define SOLAR_API __declspec(dllexport)
        #else
            #define SOLAR_API __declspec(dllimport)
        #endif
    #else
        #define SOLAR_API
    #endif
#else
    #error Solar only support Windwos!
#endif // SOLAR_PLATFORM_WINDOWS

#ifdef SOLAR_DEBUG
    #define SOLAR_ENABLE_ASSERT
#endif // SOLAR_DEBUG


#ifdef SOLAR_ENABLE_ASSERT
    #define SOLAR_ASSERT(x, ...) { if (!(x)) { SOLAR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak; } }
    #define SOLAR_CORE_ASSERT(x, ...) { if (!(x)) { SOLAR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak; } }
#else
    #define SOLAR_ASSERT(x, ...)
    #define SOLAR_CORE_ASSERT(x, ...) 
#endif

#define BIT(x) (1 << x)

#define SOLAR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Solar {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
