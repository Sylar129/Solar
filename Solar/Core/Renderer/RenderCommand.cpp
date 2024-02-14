// Copyright (c) 2024 Sylar129

#include "Core/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace solar {

Scope<RendererAPI> RenderCommand::s_renderer_api =
    CreateScope<OpenGLRendererAPI>();

}  // namespace solar
