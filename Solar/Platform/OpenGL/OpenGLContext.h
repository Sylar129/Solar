// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Renderer/GraphicsContext.h"

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
