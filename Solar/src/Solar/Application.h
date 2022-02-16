#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Solar/Events/ApplicationEvent.h"

#include "Window.h"

namespace Solar {

    class SOLAR_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e);
    private:
        bool OnWindowClose(WindowCloseEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running;
    };

    Application* CreateApplication();
}
