// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Renderer/RendererAPI.h"

namespace Solar {

class OpenGLRendererAPI : public RendererAPI {
 public:
  virtual void Init() override;
  virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) override;
  virtual void SetClearColor(const glm::vec4& color) override;
  virtual void Clear() override;

  virtual void DrawIndexed(const Ref<VertexArray>& vertex_array,
                           uint32_t index_count) override;
};

}  // namespace Solar
