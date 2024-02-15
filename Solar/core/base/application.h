// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "core/base/layer_stack.h"
#include "core/base/window.h"
#include "core/events/application_event.h"
#include "core/events/event.h"
#include "core/imgui/imgui_layer.h"

namespace solar {

class Application {
 public:
  explicit Application(const std::string& name);
  virtual ~Application();

  void Run();
  void Close();

  void OnEvent(Event& e);

  void PushLayer(Ref<Layer> layer);
  void PushOverlay(Ref<Layer> layer);

  Window& GetWindow();
  Ref<ImGuiLayer> GetImGuiLayer();

  static Application& Get();

 private:
  bool OnWindowClose(WindowCloseEvent& e);
  bool OnWindowResize(WindowResizeEvent& e);

  Scope<Window> window_;
  Ref<ImGuiLayer> imgui_layer_;
  bool running_;
  bool minimized_;
  LayerStack layer_stack_;

  static Application* instance_;
};

Application* CreateApplication();

}  // namespace solar
