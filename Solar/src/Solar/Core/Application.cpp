#include "solpch.h"
#include "Application.h"

#include "Input.h"

#include "Solar/Renderer/Renderer.h"

namespace Solar {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application()
      : m_ImGuiLayer(nullptr),
        m_Running(true),
        m_Minimized(false) {
        SOLAR_PROFILE_FUNCTION();

        SOLAR_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
        SOLAR_PROFILE_FUNCTION();
    }

    void Application::Run() {
        SOLAR_PROFILE_FUNCTION();

        while (m_Running) {
            SOLAR_PROFILE_SCOPE("RunLoop");

            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<float> duration = now - m_LastFrameTime;
            TimeStep timeStep = duration.count();
            m_LastFrameTime = now;

            /// <summary>
            /// Layer Update
            /// </summary>
            if (!m_Minimized) {
                {
                    SOLAR_PROFILE_SCOPE("LayerStack OnOpdate");

                    for (Layer* layer : m_LayerStack) {
                        layer->OnUpdate(timeStep);
                    }
                }
                
                m_ImGuiLayer->Begin();
                {
                    SOLAR_PROFILE_SCOPE("LayerStack OnImGuiRender");

                    for (Layer* layer : m_LayerStack) {
                        layer->OnImGuiRender();
                    }
                }
                m_ImGuiLayer->End();
            }


            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& e) {
        SOLAR_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
        // SOLAR_CORE_TRACE("{0}", e);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
            (*--it)->OnEvent(e);
            if (e.IsHandled()) {
                break;
            }
        }
    }

    void Application::PushLayer(Layer* layer) {
        SOLAR_PROFILE_FUNCTION();

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer) {
        SOLAR_PROFILE_FUNCTION();

        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }
    bool Application::OnWindowResize(WindowResizeEvent& e) {
        SOLAR_PROFILE_FUNCTION();

        if (e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}