#ifndef _SOLAR_APPLICATION_H_
#define _SOLAR_APPLICATION_H_
#include "Core.h"

namespace Solar {

    class SOLAR_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
    };


    Application* CreateApplication();
}

#endif // !_SOLAR_APPLICATION_H_
