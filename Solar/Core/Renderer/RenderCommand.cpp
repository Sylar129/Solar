#include "Core/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Solar {

Scope<RendererAPI> RenderCommand::s_renderer_api =
    CreateScope<OpenGLRendererAPI>();

}  // namespace Solar
