#pragma once

#ifdef SOLAR_PLATFORM_WINDOWS
    #ifdef SOLAR_BUILD_DLL
        #define SOLAR_API __declspec(dllexport)
    #else
        #define SOLAR_API __declspec(dllimport)
    #endif
#else
    #error Solar only support Windwos!
#endif // SOLAR_PLATFORM_WINDOWS

#define BIT(x) (1 << x)
