// Copyright (c) 2024 Sylar129

#include "core/base/application.h"

#include <chrono>  // NOLINT

#include "core/debug/instrumentor.h"
#include "core/renderer/renderer.h"

namespace solar {

Application* Application::instance_ = nullptr;

Application::Application(const std::string& name)
    : imgui_layer_(nullptr), running_(true), minimized_(false) {
  SOLAR_PROFILE_FUNCTION();

  SOLAR_CORE_ASSERT(!instance_, "Application already exists!");
  instance_ = this;

  // TODO(sylar): simplify
  window_ = Scope<Window>(Window::Create(WindowProps(name)));
  window_->SetEventCallback([this](Event& e) { OnEvent(e); });

  Renderer::Init();

  imgui_layer_ = CreateRef<ImGuiLayer>();
  PushOverlay(imgui_layer_);
}

Application::~Application() { SOLAR_PROFILE_FUNCTION(); }

void Application::Run() {
  SOLAR_PROFILE_FUNCTION();

  while (running_) {
    SOLAR_PROFILE_SCOPE("RunLoop");

    static auto last_frame_time = std::chrono::steady_clock::now();

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = now - last_frame_time;
    TimeStep time_step = TimeStep(duration.count());
    last_frame_time = now;

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
  dispatcher.Dispatch(this, &Application::OnWindowClose);
  dispatcher.Dispatch(this, &Application::OnWindowResize);
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
