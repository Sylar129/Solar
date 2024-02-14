// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Renderer/RendererAPI.h"

namespace solar {

class RenderCommand {
 public:
  inline static void Init() { s_renderer_api->Init(); }

  inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height) {
    s_renderer_api->SetViewPort(x, y, width, height);
  }

  inline static void SetClearColor(const glm::vec4& color) {
    s_renderer_api->SetClearColor(color);
  }

  inline static void Clear() { s_renderer_api->Clear(); }

  inline static void DrawIndexed(const Ref<VertexArray>& vertex_array,
                                 uint32_t count = 0) {
    s_renderer_api->DrawIndexed(vertex_array, count);
  }

 private:
  static Scope<RendererAPI> s_renderer_api;
};

}  // namespace solar
