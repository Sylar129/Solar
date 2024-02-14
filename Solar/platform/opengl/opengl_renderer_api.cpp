// Copyright (c) 2024 Sylar129

#include "platform/opengl/opengl_renderer_api.h"

#include "core/base/log.h"
#include "core/debug/instrumentor.h"
#include "glad/glad.h"

namespace solar {

void OpenGLMessageCallback(unsigned int source, unsigned int type,
                           unsigned int id, unsigned int serverity, int length,
                           const char* message, const void* user_param) {
  switch (serverity) {
    case GL_DEBUG_SEVERITY_HIGH:
      SOLAR_CORE_CRITICAL(message);
      return;
    case GL_DEBUG_SEVERITY_MEDIUM:
      SOLAR_CORE_ERROR(message);
      return;
    case GL_DEBUG_SEVERITY_LOW:
      SOLAR_CORE_WARN(message);
      return;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      SOLAR_CORE_TRACE(message);
      return;
  }
  SOLAR_CORE_ASSERT(false, "Unknown severity level!");
}

void OpenGLRendererAPI::Init() {
  SOLAR_PROFILE_FUNCTION();

#ifdef SOLAR_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(OpenGLMessageCallback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                        GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif  // SOLAR_DEBUG

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height) {
  glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertex_array,
                                    uint32_t index_count) {
  uint32_t count = index_count != 0
                       ? index_count
                       : vertex_array->GetIndexBuffer()->GetCount();

  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

}  // namespace solar
