#pragma once

#include <GLFW/glfw3.h>

#include "Core/Base/Window.h"
#include "Core/Renderer/GraphicsContext.h"

namespace Solar {

class WindowsWindow : public Window {
 public:
  WindowsWindow(const WindowProps& props);
  virtual ~WindowsWindow();

  void OnUpdate() override;

  inline uint32_t GetWidth() const override { return m_Data.Width; }
  inline uint32_t GetHeight() const override { return m_Data.Height; }

  // Windows attributes
  inline void SetEventCallback(const EventCallbackFn& callback) override {
    m_Data.EventCallback = callback;
  }
  void SetVSync(bool enabled) override;
  bool IsVSync() const override;

  inline virtual void* GetNativeWindow() const override { return m_Window; }

 private:
  virtual void Init(const WindowProps& props);
  virtual void Shutdown();

 private:
  GLFWwindow* m_Window;
  Scope<GraphicsContext> m_Context;

  struct WindowData {
    std::string Title;
    uint32_t Width;
    uint32_t Height;
    bool VSync;
    EventCallbackFn EventCallback;

    WindowData() : Width(0), Height(0), VSync(false) {}
  };

  WindowData m_Data;
};

}  // namespace Solar
