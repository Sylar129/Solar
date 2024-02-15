// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "GLFW/glfw3.h"
#include "core/base/window.h"
#include "core/renderer/graphics_context.h"
#include "core/utils/misc.h"

namespace solar {

class WindowsWindow : public Window {
 public:
  explicit WindowsWindow(const WindowProps& props);
  ~WindowsWindow() override;

  void OnUpdate() override;

  uint32_t GetWidth() const override;
  uint32_t GetHeight() const override;

  // Windows attributes
  void SetEventCallback(const EventCallbackFn& callback) override;
  void SetVSync(bool enabled) override;
  bool IsVSync() const override;

  void* GetNativeWindow() const override;

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
