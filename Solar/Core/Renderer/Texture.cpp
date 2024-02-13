// Copyright (c) 2024 Sylar129

#include "Core/Renderer/Texture.h"

#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Solar {

Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::kNone:
      SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::kOpenGL:
      return CreateRef<OpenGLTexture2D>(width, height);
  }
  SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string& path) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::kNone:
      SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::kOpenGL:
      return CreateRef<OpenGLTexture2D>(path);
  }
  SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

}  // namespace Solar
