#include "solpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

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
        io.DisplaySize = ImVec2(1280, 720);

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

    }
}
