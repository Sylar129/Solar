#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {
}

void Sandbox2D::OnAttach() {

}

void Sandbox2D::OnDetech() {

}

void Sandbox2D::OnUpdate(Solar::TimeStep& ts) {
    // SOLAR_INFO("ExampleLayer::Update");
    // SOLAR_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

    m_CameraController.OnUpdate(ts);

    /// <summary>
    /// Renderer
    /// </summary>
    Solar::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Solar::RenderCommand::Clear();

    Solar::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Solar::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });

    Solar::Renderer2D::EndScene();
    //static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    //std::dynamic_pointer_cast<Solar::OpenGLShader>(m_FlatColorShader)->Bind();
    //std::dynamic_pointer_cast<Solar::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

    //Solar::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

}

void Sandbox2D::OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Solar::Event& event) {
    m_CameraController.OnEvent(event);
}
