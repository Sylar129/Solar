// Copyright (c) 2024 Sylar129

#pragma once

#include "core/base/layer.h"

namespace solar {

class ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer();

  void OnAttach() override;
  void OnDetech() override;
  void OnEvent(Event& e) override;

  void Begin();
  void End();

  void SetBlockEvents(bool block);

  void SetDarkThemeColors();

 private:
  bool block_events_ = true;
  float time_;
};

}  // namespace solar
