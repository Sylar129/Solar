#include "solpch.h"
#include "Application.h"

#include "Solar/Events/ApplicationEvent.h"
#include "Solar/Log.h"

#include "GLFW/glfw3.h"

namespace Solar {
    Application::Application()
        : m_Running(true) {
        m_Window = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application() {

    }

    void Application::Run() {
        while (m_Running) {
            glClearColor(1, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }
}
