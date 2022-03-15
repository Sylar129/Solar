#pragma once

#include "Solar/Core/Window.h"
#include "Solar/Core/LayerStack.h"
#include "Solar/Events/Event.h"
#include "Solar/Events/ApplicationEvent.h"

#include "Solar/Core/Timestep.h"

#include "Solar/ImGui/ImGuiLayer.h"

namespace Solar {

    class SOLAR_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline Window& GetWindow() {
            return *m_Window;
        }

        inline static Application& Get() {
            return *s_Instance;
        }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
    private:
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running;
        bool m_Minimized;
        LayerStack m_LayerStack;
        std::chrono::steady_clock::time_point m_LastFrameTime;
    private:
        static Application* s_Instance;
    };

    Application* CreateApplication();
}
