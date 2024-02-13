// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "Core/Events/Event.h"

namespace Solar {

struct WindowProps {
  std::string title;
  uint32_t width;
  uint32_t height;

  explicit WindowProps(const std::string& title = "Solar Engine",
                       uint32_t width = 1600, uint32_t height = 900)
      : title(title), width(width), height(height) {}
};

// Interface representing a desktop system based window
class Window {
 public:
  using EventCallbackFn = std::function<void(Event&)>;

  virtual ~Window() {}

  virtual void OnUpdate() = 0;

  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;

  // Window attributes
  virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;

  virtual void* GetNativeWindow() const = 0;

  static Window* Create(const WindowProps& props = WindowProps());
};

}  // namespace Solar
