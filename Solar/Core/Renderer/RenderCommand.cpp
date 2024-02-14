// Copyright (c) 2024 Sylar129

#include "Core/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace solar {

Scope<RendererAPI> RenderCommand::renderer_api_ =
    CreateScope<OpenGLRendererAPI>();

}  // namespace solar
