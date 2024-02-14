// Copyright (c) 2024 Sylar129

#include "platform/windows/windows_window.h"

#include <Windows.h>

#include "core/base/log.h"
#include "core/debug/instrumentor.h"
#include "core/events/application_event.h"
#include "core/events/key_event.h"
#include "core/events/mouse_event.h"
#include "core/renderer/renderer.h"
#include "platform/opengl/opengl_context.h"

namespace solar {

namespace {

uint8_t s_GLFWWindowCount = 0;

void GLFWErrorCallback(int error, const char* description) {
  SOLAR_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

}  // namespace

Window* Window::Create(const WindowProps& props) {
  return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
  SOLAR_PROFILE_FUNCTION();

  Init(props);
}

WindowsWindow::~WindowsWindow() {
  SOLAR_PROFILE_FUNCTION();

  Shutdown();
}

void WindowsWindow::OnUpdate() {
  SOLAR_PROFILE_FUNCTION();

  glfwPollEvents();
  context_->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled) {
  SOLAR_PROFILE_FUNCTION();

  if (enabled) {
    glfwSwapInterval(1);
  } else {
    glfwSwapInterval(0);
  }
  data_.v_sync = enabled;
}

bool WindowsWindow::IsVSync() const { return data_.v_sync; }

void WindowsWindow::Init(const WindowProps& props) {
  SOLAR_PROFILE_FUNCTION();

  data_.title = props.title;
  data_.width = props.width;
  data_.height = props.height;

  SOLAR_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width,
                  props.height);

  if (s_GLFWWindowCount == 0) {
    SOLAR_PROFILE_SCOPE("glfwInit");

    SOLAR_CORE_INFO("Initializing GLFW");
    int success = glfwInit();
    SOLAR_CORE_ASSERT(success, "Could not intiliaze GLFW!");
    glfwSetErrorCallback(GLFWErrorCallback);
  }
  {
    SOLAR_PROFILE_SCOPE("glfwCreateWindow");

#ifdef SOLAR_DEBUG
    if (Renderer::GetAPI() == RendererAPI::API::kOpenGL) {
      glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    }
#endif  // SOLAR_DEBUG

    window_ = glfwCreateWindow(static_cast<int>(props.width),
                               static_cast<int>(props.height),
                               props.title.c_str(), nullptr, nullptr);
    s_GLFWWindowCount++;
  }

  context_ = CreateScope<OpenGLContext>(window_);
  context_->Init();

  glfwSetWindowUserPointer(window_, &data_);
  SetVSync(true);

  // Set GLFW callbacks
  glfwSetWindowSizeCallback(
      window_, [](GLFWwindow* window, int width, int height) {
        WindowData& data =
            *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.width = width;
        data.height = height;

        WindowResizeEvent event(width, height);
        data.event_callback(event);
      });

  glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
    WindowData& data =
        *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

    WindowCloseEvent event;
    data.event_callback(event);
  });

  glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode,
                                 int action, int mods) {
    WindowData& data =
        *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

    KeyCode key_code = static_cast<KeyCode>(key);
    switch (action) {
      case GLFW_PRESS: {
        KeyPressdEvent event(key_code, 0);
        data.event_callback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key_code);
        data.event_callback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressdEvent event(key_code, 1);
        data.event_callback(event);
        break;
      }
      default:
        break;
    }
  });

  glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int codepoint) {
    WindowData& data =
        *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

    KeyTypedEvent event(static_cast<KeyCode>(codepoint));
    data.event_callback(event);
  });

  glfwSetMouseButtonCallback(
      window_, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data =
            *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        MouseCode mouse_code = static_cast<MouseCode>(button);
        switch (action) {
          case GLFW_PRESS: {
            MouseButtonPressedEvent event(mouse_code);
            data.event_callback(event);
            break;
          }
          case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(mouse_code);
            data.event_callback(event);
            break;
          }
          default:
            break;
        }
      });

  glfwSetScrollCallback(
      window_, [](GLFWwindow* window, double x_offset, double y_offset) {
        WindowData& data =
            *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        MouseScrolledEvent event(static_cast<float>(x_offset),
                                 static_cast<float>(y_offset));
        data.event_callback(event);
      });

  glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double x_pos,
                                       double y_pos) {
    WindowData& data =
        *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

    MouseMovedEvent event(static_cast<float>(x_pos), static_cast<float>(y_pos));
    data.event_callback(event);
  });
}

void WindowsWindow::Shutdown() {
  SOLAR_PROFILE_FUNCTION();

  glfwDestroyWindow(window_);
  s_GLFWWindowCount--;
  if (s_GLFWWindowCount == 0) {
    SOLAR_CORE_INFO("Terminating GLFW");
    glfwTerminate();
  }
}

}  // namespace solar
