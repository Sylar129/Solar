#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Solar {

    class SOLAR_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
    };


    Application* CreateApplication();
}
