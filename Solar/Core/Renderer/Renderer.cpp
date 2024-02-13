// Copyright (c) 2024 Sylar129

#include "Core/Renderer/Renderer.h"

#include "Core/Debug/Instrumentor.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Solar {

Scope<Renderer::SceneData> Renderer::s_scene_data =
    CreateScope<Renderer::SceneData>();

void Renderer::Init() {
  SOLAR_PROFILE_FUNCTION();

  RenderCommand::Init();
  Renderer2D::Init();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
  RenderCommand::SetViewPort(0, 0, width, height);
}

void Renderer::BeginScene(const OrthographicCamera& camera) {
  s_scene_data->view_projection_matrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader>& shader,
                      const Ref<VertexArray>& vertex_array,
                      const glm::mat4& transform) {
  shader->Bind();
  std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(
      "u_ViewProjection", s_scene_data->view_projection_matrix);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(
      "u_Transform", transform);

  vertex_array->Bind();
  RenderCommand::DrawIndexed(vertex_array);
}

}  // namespace Solar
