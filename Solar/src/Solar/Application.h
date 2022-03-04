#pragma once

#include "Window.h"
#include "Solar/LayerStack.h"
#include "Solar/Events/Event.h"
#include "Solar/Events/ApplicationEvent.h"

#include "Solar/ImGui/ImGuiLayer.h"

#include "Solar/Renderer/Shader.h"
#include "Solar/Renderer/Buffer.h"

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
    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running;
        LayerStack m_LayerStack;

        unsigned int m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
    private:
        static Application* s_Instance;
    };

    Application* CreateApplication();
}
