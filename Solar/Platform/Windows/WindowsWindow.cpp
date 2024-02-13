#include "Platform/Windows/WindowsWindow.h"

#include <Windows.h>

#include "Core/Base/Input.h"
#include "Core/Base/Log.h"
#include "Core/Debug/Instrumentor.h"
#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Solar {

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
  m_Context->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled) {
  SOLAR_PROFILE_FUNCTION();

  if (enabled) {
    glfwSwapInterval(1);
  } else {
    glfwSwapInterval(0);
  }
  m_Data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const { return m_Data.VSync; }

void WindowsWindow::Init(const WindowProps& props) {
  SOLAR_PROFILE_FUNCTION();

  m_Data.Title = props.title;
  m_Data.Width = props.width;
  m_Data.Height = props.height;

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
    if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {
      glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    }
#endif  // SOLAR_DEBUG

    m_Window = glfwCreateWindow((int)props.width, (int)props.height,
                                props.title.c_str(), nullptr, nullptr);
    s_GLFWWindowCount++;
  }

  m_Context = CreateScope<OpenGLContext>(m_Window);
  m_Context->Init();

  glfwSetWindowUserPointer(m_Window, &m_Data);
  SetVSync(true);

  // Set GLFW callbacks
  glfwSetWindowSizeCallback(
      m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
      });

  glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data.EventCallback(event);
  });

  glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode,
                                  int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    KeyCode key_code = static_cast<KeyCode>(key);
    switch (action) {
      case GLFW_PRESS: {
        KeyPressdEvent event(key_code, 0);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key_code);
        data.EventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressdEvent event(key_code, 1);
        data.EventCallback(event);
        break;
      }
      default:
        break;
    }
  });

  glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    KeyTypedEvent event(static_cast<KeyCode>(codepoint));
    data.EventCallback(event);
  });

  glfwSetMouseButtonCallback(
      m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseCode mouse_code = static_cast<MouseCode>(button);
        switch (action) {
          case GLFW_PRESS: {
            MouseButtonPressedEvent event(mouse_code);
            data.EventCallback(event);
            break;
          }
          case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(mouse_code);
            data.EventCallback(event);
            break;
          }
          default:
            break;
        }
      });

  glfwSetScrollCallback(
      m_Window, [](GLFWwindow* window, double x_offset, double y_offset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event((float)x_offset, (float)y_offset);
        data.EventCallback(event);
      });

  glfwSetCursorPosCallback(
      m_Window, [](GLFWwindow* window, double x_pos, double y_pos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event((float)x_pos, (float)y_pos);
        data.EventCallback(event);
      });
}

void WindowsWindow::Shutdown() {
  SOLAR_PROFILE_FUNCTION();

  glfwDestroyWindow(m_Window);
  s_GLFWWindowCount--;
  if (s_GLFWWindowCount == 0) {
    SOLAR_CORE_INFO("Terminating GLFW");
    glfwTerminate();
  }
}

}  // namespace Solar
