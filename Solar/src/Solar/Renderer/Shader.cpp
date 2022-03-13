#include "solpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Solar {
    Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
        }
        SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}
