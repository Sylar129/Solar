// Copyright (c) 2024 Sylar129

#include "core/renderer/renderer.h"

#include "core/debug/instrumentor.h"
#include "core/renderer/render_command.h"
#include "core/renderer/renderer2d.h"
#include "platform/opengl/shader.h"

namespace solar {

Scope<Renderer::SceneData> Renderer::scene_data_ =
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
  scene_data_->view_projection_matrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader>& shader,
                      const Ref<VertexArray>& vertex_array,
                      const glm::mat4& transform) {
  shader->Bind();
  std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(
      "u_ViewProjection", scene_data_->view_projection_matrix);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(
      "u_Transform", transform);

  vertex_array->Bind();
  RenderCommand::DrawIndexed(vertex_array);
}

}  // namespace solar
