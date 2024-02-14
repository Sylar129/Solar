// Copyright (c) 2024 Sylar129

#pragma once

namespace solar {

class GraphicsContext {
 public:
  virtual ~GraphicsContext() = default;

  virtual void Init() = 0;
  virtual void SwapBuffers() = 0;
};

}  // namespace solar
