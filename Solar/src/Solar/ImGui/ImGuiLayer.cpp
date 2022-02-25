#include "solpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"

// Temporary
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Solar/Application.h"

namespace Solar {
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer"), m_Time(0.0f) {

    }

    ImGuiLayer::~ImGuiLayer() {

    }

    void ImGuiLayer::OnAttach() {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetech() {
    }

    void ImGuiLayer::OnUpdate() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnEvent(Event& event) {
        SOLAR_CORE_TRACE("ImGuiLayer: {0}", event);
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(SOLAR_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(SOLAR_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(SOLAR_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(SOLAR_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressdEvent>(SOLAR_BIND_EVENT_FN(ImGuiLayer::OnKeyPressdEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(SOLAR_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(SOLAR_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(SOLAR_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
        // Cherno Style
        //ImGuiIO& io = ImGui::GetIO();
        //io.MouseDown[e.GetMouseButton()] = true;

        // ImGui Style
        ImGuiIO& io = ImGui::GetIO();
        if (e.GetMouseButton() >= 0 && e.GetMouseButton() < ImGuiMouseButton_COUNT)
            io.AddMouseButtonEvent(e.GetMouseButton(), true);

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {
        // Cherno Style
        //ImGuiIO& io = ImGui::GetIO();
        //io.MouseDown[e.GetMouseButton()] = false;

        // ImGui Style
        ImGuiIO& io = ImGui::GetIO();
        if (e.GetMouseButton() >= 0 && e.GetMouseButton() < ImGuiMouseButton_COUNT)
            io.AddMouseButtonEvent(e.GetMouseButton(), false);

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e) {
        // Cherno Style
        //ImGuiIO& io = ImGui::GetIO();
        //io.MousePos = ImVec2(e.GetX(), e.GetY());

        // ImGui Style
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent((float)e.GetX(), (float)e.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e) {
        // Cherno Style
        //ImGuiIO& io = ImGui::GetIO();
        //io.MouseWheelH += e.GetXOffset();
        //io.MouseWheel += e.GetYOffset();

        // ImGui Style
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseWheelEvent((float)e.GetXOffset(), (float)e.GetYOffset());

        return false;
    }

    bool ImGuiLayer::OnKeyPressdEvent(KeyPressdEvent& e) {
        // Cherno Style
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        // ImGui Style
        //ImGuiIO& io = ImGui::GetIO();
        //ImGuiKey imgui_key = ImGui_ImplGlfw_KeyToImGuiKey(e.GetKeyCode());
        //io.AddKeyEvent(imgui_key, true);

        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) {
        // Cherno Style
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;

        // ImGui Style
        //ImGuiIO& io = ImGui::GetIO();
        //ImGuiKey imgui_key = ImGui_ImplGlfw_KeyToImGuiKey(e.GetKeyCode());
        //io.AddKeyEvent(imgui_key, false);

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharacter(e.GetKeyCode());

        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.GetWidth(), e.GetHeight());

        return false;
    }
}
