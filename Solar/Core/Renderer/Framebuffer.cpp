// Copyright (c) 2024 Sylar129

#include "Core/Renderer/Framebuffer.h"

#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Solar {

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::kNone:
      SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::kOpenGL:
      return CreateRef<OpenGLFramebuffer>(spec);
  }
  SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

}  // namespace Solar
