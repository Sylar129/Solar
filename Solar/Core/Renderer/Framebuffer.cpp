#include "solpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Solar {

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        SOLAR_CORE_ASSERT(false,
                          "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLFramebuffer>(spec);
    }
    SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace Solar
