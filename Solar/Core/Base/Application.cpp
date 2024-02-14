// Copyright (c) 2024 Sylar129

#include "Core/Base/Application.h"

#include <chrono>

#include "Core/Debug/Instrumentor.h"
#include "Core/Renderer/Renderer.h"

namespace solar {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name)
    : imgui_layer_(nullptr), running_(true), minimized_(false) {
  SOLAR_PROFILE_FUNCTION();

  SOLAR_CORE_ASSERT(!s_Instance, "Application already exists!");
  s_Instance = this;

  // TODO(sylar): simplify
  window_ = Scope<Window>(Window::Create(WindowProps(name)));
  window_->SetEventCallback(BIND_EVENT_FN(OnEvent));

  Renderer::Init();

  imgui_layer_ = CreateRef<ImGuiLayer>();
  PushOverlay(imgui_layer_);
}

Application::~Application() { SOLAR_PROFILE_FUNCTION(); }

void Application::Run() {
  SOLAR_PROFILE_FUNCTION();

  while (running_) {
    SOLAR_PROFILE_SCOPE("RunLoop");

    static auto s_last_frame_time = std::chrono::steady_clock::now();

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = now - s_last_frame_time;
    TimeStep time_step = TimeStep(duration.count());
    s_last_frame_time = now;

    /// <summary>
    /// Layer Update
    /// </summary>
    if (!minimized_) {
      {
        SOLAR_PROFILE_SCOPE("LayerStack OnOpdate");

        for (auto& layer : layer_stack_) {
          layer->OnUpdate(time_step);
        }
      }

      imgui_layer_->Begin();
      {
        SOLAR_PROFILE_SCOPE("LayerStack OnImGuiRender");

        for (auto& layer : layer_stack_) {
          layer->OnImGuiRender();
        }
      }
      imgui_layer_->End();
    }

    window_->OnUpdate();
  }
}

void Application::OnEvent(Event& e) {
  SOLAR_PROFILE_FUNCTION();

  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
  dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
  // SOLAR_CORE_TRACE("{0}", e);

  for (auto it = layer_stack_.rbegin(); it != layer_stack_.rend(); ++it) {
    (*it)->OnEvent(e);
    if (e.handled) {
      break;
    }
  }
}

void Application::PushLayer(Ref<Layer> layer) {
  SOLAR_PROFILE_FUNCTION();

  layer_stack_.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Ref<Layer> layer) {
  SOLAR_PROFILE_FUNCTION();

  layer_stack_.PushOverlay(layer);
  layer->OnAttach();
}

void Application::Close() { running_ = false; }

bool Application::OnWindowClose(WindowCloseEvent& e) {
  running_ = false;
  return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
  SOLAR_PROFILE_FUNCTION();

  if (e.GetWidth() == 0 || e.GetHeight() == 0) {
    minimized_ = true;
    return false;
  }

  minimized_ = false;
  Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
  return false;
}

}  // namespace solar
