// Copyright (c) 2024 Sylar129

#include "core/renderer/vertex_array.h"

#include "core/renderer/renderer.h"
#include "platform/opengl/opengl_vertex_array.h"

namespace solar {

Ref<VertexArray> VertexArray::Create() {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::kNone:
      SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::kOpenGL:
      return CreateRef<OpenGLVertexArray>();
  }
  SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

}  // namespace solar
