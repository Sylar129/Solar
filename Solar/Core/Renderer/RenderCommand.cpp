#include "Core/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Solar {

Scope<RendererAPI> RenderCommand::s_RendererAPI =
    CreateScope<OpenGLRendererAPI>();

} // namespace Solar
