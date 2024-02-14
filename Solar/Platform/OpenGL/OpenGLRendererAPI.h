// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Renderer/RendererAPI.h"

namespace solar {

class OpenGLRendererAPI : public RendererAPI {
 public:
  void Init() override;
  void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                   uint32_t height) override;
  void SetClearColor(const glm::vec4& color) override;
  void Clear() override;

  void DrawIndexed(const Ref<VertexArray>& vertex_array,
                   uint32_t index_count) override;
};

}  // namespace solar
