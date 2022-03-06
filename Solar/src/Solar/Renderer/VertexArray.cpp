#include "solpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Solar {
    VertexArray* VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::None:
                SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::OpenGL:
                return new OpenGLVertexArray();
        }
        SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}
