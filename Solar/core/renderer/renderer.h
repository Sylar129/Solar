// Copyright (c) 2024 Sylar129

#pragma once

#include "core/renderer/orthographic_camera.h"
#include "core/renderer/renderer_api.h"
#include "core/renderer/shader.h"

namespace solar {

class Renderer {
 public:
  static void Init();
  static void OnWindowResize(uint32_t width, uint32_t height);

  static void BeginScene(const OrthographicCamera& camera);
  static void EndScene();

  static void Submit(const Ref<Shader>& shader,
                     const Ref<VertexArray>& vertex_array,
                     const glm::mat4& transform = glm::mat4(1.0f));

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

 private:
  struct SceneData {
    glm::mat4 view_projection_matrix;
  };

  static Scope<SceneData> scene_data_;
};

}  // namespace solar
