// Copyright (c) 2024 Sylar129

#include "core/renderer/render_command.h"

#include "platform/opengl/renderer_api.h"

namespace solar {

// TODO(sylar): bind to a class instead of static duration
Scope<RendererAPI> RenderCommand::renderer_api_ =
    CreateScope<OpenGLRendererAPI>();

void RenderCommand::Init() { renderer_api_->Init(); }

void RenderCommand::SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                                uint32_t height) {
  renderer_api_->SetViewPort(x, y, width, height);
}

void RenderCommand::SetClearColor(const glm::vec4& color) {
  renderer_api_->SetClearColor(color);
}

void RenderCommand::Clear() { renderer_api_->Clear(); }

void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertex_array,
                                uint32_t count) {
  renderer_api_->DrawIndexed(vertex_array, count);
}

}  // namespace solar
