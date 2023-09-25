#include "Core/Renderer/VertexArray.h"

#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Solar {

Ref<VertexArray> VertexArray::Create()
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        SOLAR_CORE_ASSERT(false,
                          "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexArray>();
    }
    SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

} // namespace Solar
