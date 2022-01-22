#ifndef _SOLAR_SOLAR_CORE_H_
#define _SOLAR_SOLAR_CORE_H_

#ifdef SOLAR_PLATFORM_WINDOWS
    #ifdef SOLAR_BUILD_DLL
        #define SOLAR_API __declspec(dllexport)
    #else
        #define SOLAR_API __declspec(dllimport)
    #endif
#else
    #error Solar only support Windwos!
#endif // SOLAR_PLATFORM_WINDOWS

#endif // _SOLAR_SOLAR_CORE_H_
