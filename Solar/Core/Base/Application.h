#pragma once

#include "Core/Base/Window.h"
#include "Core/Base/LayerStack.h"
#include "Core/Events/Event.h"
#include "Core/Events/ApplicationEvent.h"

#include "Core/Base/Timestep.h"

#include "Core/ImGui/ImGuiLayer.h"

namespace Solar {

class Application {
public:
    Application(const std::string& name = "Solar App");
    virtual ~Application();

    void Run();
    void OnEvent(Event& e);

    void PushLayer(Ref<Layer> layer);
    void PushOverlay(Ref<Layer> layer);

    inline Window& GetWindow() { return *m_Window; }

    void Close();

    Ref<ImGuiLayer> GetImGuiLayer() { return m_ImGuiLayer; }

    inline static Application& Get() { return *s_Instance; }

private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

private:
    Scope<Window> m_Window;
    Ref<ImGuiLayer> m_ImGuiLayer;
    bool m_Running;
    bool m_Minimized;
    LayerStack m_LayerStack;

private:
    static Application* s_Instance;
};

Application* CreateApplication();

} // namespace Solar
