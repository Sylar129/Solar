// Copyright (c) 2024 Sylar129

#include "platform/opengl/context.h"

// clang-format off
#include "core/debug/assert.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include "core/debug/instrumentor.h"

namespace solar {

OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
    : window_handle_(window_handle) {
  SOLAR_CORE_ASSERT(window_handle, "Window handle is null!");
}

void OpenGLContext::Init() {
  SOLAR_PROFILE_FUNCTION();

  glfwMakeContextCurrent(window_handle_);
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  SOLAR_CORE_ASSERT(status, "Failed to initialize Glad!");

  SOLAR_CORE_INFO("OpenGL Info:");
  SOLAR_CORE_INFO("    Vendor: {0}", (char*)glGetString(GL_VENDOR));
  SOLAR_CORE_INFO("    Renderer: {0}", (char*)glGetString(GL_RENDERER));
  SOLAR_CORE_INFO("    Version: {0}", (char*)glGetString(GL_VERSION));

#ifdef SOLAR_ENABLE_ASSERT
  int versionMajor;
  int versionMinor;
  glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
  glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

  SOLAR_CORE_ASSERT(
      versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
      "Solar requires at least OpenGL version 4.5!");
#endif
}

void OpenGLContext::SwapBuffers() {
  SOLAR_PROFILE_FUNCTION();

  glfwSwapBuffers(window_handle_);
}

}  // namespace solar
