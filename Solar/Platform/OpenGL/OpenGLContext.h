// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Solar {

class OpenGLContext : public GraphicsContext {
 public:
  explicit OpenGLContext(GLFWwindow* window_handle);

  virtual void Init() override;
  virtual void SwapBuffers() override;

 private:
  GLFWwindow* window_handle_;
};

}  // namespace Solar
