#include "solpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Solar {
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::None:
                SOLAR_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::OpenGL:
                return new OpenGLVertexBuffer(vertices, size);
        }
        SOLAR_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::None:
                SOLAR_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::OpenGL:
                return new OpenGLIndexBuffer(indices, count);
        }
        SOLAR_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}
