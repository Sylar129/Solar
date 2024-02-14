// Copyright (c) 2024 Sylar129

#include "core/renderer/render_command.h"

#include "platform/opengl/renderer_api.h"

namespace solar {

Scope<RendererAPI> RenderCommand::renderer_api_ =
    CreateScope<OpenGLRendererAPI>();

}  // namespace solar
