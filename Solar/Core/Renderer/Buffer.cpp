// Copyright (c) 2024 Sylar129

#include "Core/Renderer/Buffer.h"

#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Solar {

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::kNone:
      SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::kOpenGL:
      return CreateRef<OpenGLVertexBuffer>(size);
  }
  SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}
Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::kNone:
      SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::kOpenGL:
      return CreateRef<OpenGLVertexBuffer>(vertices, size);
  }
  SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::kNone:
      SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::kOpenGL:
      return CreateRef<OpenGLIndexBuffer>(indices, count);
  }
  SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

}  // namespace Solar
