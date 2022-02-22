#pragma once

#include "Solar/Events/Event.h"
#include "Solar/Events/ApplicationEvent.h"
#include "Solar/LayerStack.h"
#include "Window.h"

namespace Solar {

    class SOLAR_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline Window& GetWindow() const {
            return *m_Window;
        }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running;
        LayerStack m_LayerStack;
    };

    Application* CreateApplication();
}
