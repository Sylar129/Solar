#include "Core/Base/Application.h"

#include <chrono>

#include "Core/Base/Input.h"
#include "Core/Debug/Instrumentor.h"
#include "Core/Renderer/Renderer.h"

namespace Solar {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name)
    : m_ImGuiLayer(nullptr), m_Running(true), m_Minimized(false)
{
    SOLAR_PROFILE_FUNCTION();

    SOLAR_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

    Renderer::Init();

    m_ImGuiLayer = CreateRef<ImGuiLayer>();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() { SOLAR_PROFILE_FUNCTION(); }

void Application::Run()
{
    SOLAR_PROFILE_FUNCTION();

    while (m_Running) {
        SOLAR_PROFILE_SCOPE("RunLoop");

        static auto lastFrameTime = std::chrono::steady_clock::now();

        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> duration = now - lastFrameTime;
        TimeStep timeStep = duration.count();
        lastFrameTime = now;

        /// <summary>
        /// Layer Update
        /// </summary>
        if (!m_Minimized) {
            {
                SOLAR_PROFILE_SCOPE("LayerStack OnOpdate");

                for (auto& layer : m_LayerStack) {
                    layer->OnUpdate(timeStep);
                }
            }

            m_ImGuiLayer->Begin();
            {
                SOLAR_PROFILE_SCOPE("LayerStack OnImGuiRender");

                for (auto& layer : m_LayerStack) {
                    layer->OnImGuiRender();
                }
            }
            m_ImGuiLayer->End();
        }

        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& e)
{
    SOLAR_PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
    // SOLAR_CORE_TRACE("{0}", e);

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
        (*it)->OnEvent(e);
        if (e.Handled) {
            break;
        }
    }
}

void Application::PushLayer(Ref<Layer> layer)
{
    SOLAR_PROFILE_FUNCTION();

    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Ref<Layer> layer)
{
    SOLAR_PROFILE_FUNCTION();

    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::Close() { m_Running = false; }

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_Running = false;
    return true;
}
bool Application::OnWindowResize(WindowResizeEvent& e)
{
    SOLAR_PROFILE_FUNCTION();

    if (e.GetWidth() == 0 || e.GetHeight() == 0) {
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
    return false;
}

} // namespace Solar
