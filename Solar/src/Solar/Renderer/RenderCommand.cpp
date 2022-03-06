#include "solpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Solar {
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}
