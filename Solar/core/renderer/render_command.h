// Copyright (c) 2024 Sylar129

#pragma once

#include "core/renderer/renderer_api.h"

namespace solar {

class RenderCommand {
 public:
  static void Init();

  static void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                          uint32_t height);

  static void SetClearColor(const glm::vec4& color);

  static void Clear();

  static void DrawIndexed(const Ref<VertexArray>& vertex_array,
                          uint32_t count = 0);

 private:
  static Scope<RendererAPI> renderer_api_;
};

}  // namespace solar
