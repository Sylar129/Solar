#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Platform/OpenGL/OpenGLShader.h>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {
}

void Sandbox2D::OnAttach() {
    SOLAR_PROFILE_FUNCTION();

    m_BoardTexture = Solar::Texture2D::Create("assets/textures/Board.png");
}

void Sandbox2D::OnDetech() {
    SOLAR_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Solar::TimeStep& ts) {
    // SOLAR_INFO("ExampleLayer::Update");
    // SOLAR_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

    SOLAR_PROFILE_FUNCTION();


    m_CameraController.OnUpdate(ts);

    /// <summary>
    /// Renderer
    /// </summary>
    Solar::Renderer2D::ResetStats();
    {
        SOLAR_PROFILE_SCOPE("Renderer Prep");

        Solar::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Solar::RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 20.0f;
        rotation = rotation > 360.0f ? rotation - 360.0f : rotation;
        SOLAR_PROFILE_SCOPE("Renderer Draw");

        Solar::Renderer2D::BeginScene(m_CameraController.GetCamera());
        Solar::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Solar::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.5f }, { 0.3f, 0.2f, 0.8f, 1.0f });
        Solar::Renderer2D::DrawRotateQuad({ 0.5f, -0.5f }, { 0.5f, 0.5f }, 45.0f, { 0.3f, 0.8f, 0.2f, 1.0f });
        Solar::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_BoardTexture, 10.0f);
        Solar::Renderer2D::DrawRotateQuad({ 0.0f, 1.0f }, { 1.0f, 1.0f }, rotation, m_BoardTexture, 20.0f);
        Solar::Renderer2D::EndScene();

        Solar::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f) {
            for (float x = -5.0f; x < 5.0f; x += 0.5f) {
                glm::vec4 color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.4f, 0.7f };
                Solar::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        }
        Solar::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender() {
    SOLAR_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
    auto stats = Solar::Renderer2D::GetStats();
    ImGui::Text("Renderer2D stats:");
    ImGui::Text("Draw calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Solar::Event& event) {
    // SOLAR_TRACE("ExampleLayer: {0}", event);
    m_CameraController.OnEvent(event);
}
