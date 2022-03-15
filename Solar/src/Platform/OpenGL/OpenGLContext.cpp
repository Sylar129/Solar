#include "solpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Solar {
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
      : m_WindowHandle(windowHandle) {
        SOLAR_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init() {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        SOLAR_CORE_ASSERT(status, "Failed to initialize Glad!");

        SOLAR_CORE_INFO("OpenGL Info:");
        SOLAR_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
        SOLAR_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
        SOLAR_CORE_INFO("    Version: {0}", glGetString(GL_VERSION));

#ifdef SOLAR_ENABLE_ASSERT
        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        SOLAR_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");
#endif
    }

    void OpenGLContext::SwapBuffers() {
        glfwSwapBuffers(m_WindowHandle);
    }
}
