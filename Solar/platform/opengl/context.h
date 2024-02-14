// Copyright (c) 2024 Sylar129

#pragma once

#include "core/renderer/graphics_context.h"

struct GLFWwindow;

namespace solar {

class OpenGLContext : public GraphicsContext {
 public:
  explicit OpenGLContext(GLFWwindow* window_handle);

  void Init() override;
  void SwapBuffers() override;

 private:
  GLFWwindow* window_handle_;
};

}  // namespace solar
