// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "GLFW/glfw3.h"
#include "core/base/window.h"
#include "core/renderer/graphics_context.h"

namespace solar {

class WindowsWindow : public Window {
 public:
  explicit WindowsWindow(const WindowProps& props);
  ~WindowsWindow() override;

  void OnUpdate() override;

  inline uint32_t GetWidth() const override { return data_.width; }
  inline uint32_t GetHeight() const override { return data_.height; }

  // Windows attributes
  inline void SetEventCallback(const EventCallbackFn& callback) override {
    data_.event_callback = callback;
  }
  void SetVSync(bool enabled) override;
  bool IsVSync() const override;

  inline void* GetNativeWindow() const override { return window_; }

 private:
  virtual void Init(const WindowProps& props);
  virtual void Shutdown();

  GLFWwindow* window_;
  Scope<GraphicsContext> context_;

  struct WindowData {
    std::string title;
    uint32_t width;
    uint32_t height;
    bool v_sync;
    EventCallbackFn event_callback;

    WindowData() : width(0), height(0), v_sync(false) {}
  };

  WindowData data_;
};

}  // namespace solar