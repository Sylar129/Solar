// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Renderer/VertexArray.h"
#include "glm/vec4.hpp"

namespace Solar {

class RendererAPI {
 public:
  enum class API { kNone = 0, kOpenGL = 1 };

  virtual ~RendererAPI() = default;

  virtual void Init() = 0;
  virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) = 0;
  virtual void SetClearColor(const glm::vec4& color) = 0;
  virtual void Clear() = 0;

  virtual void DrawIndexed(const Ref<VertexArray>& vertex_array,
                           uint32_t index_count) = 0;

  inline static API GetAPI() { return s_api; }

 private:
  static API s_api;
};

}  // namespace Solar
