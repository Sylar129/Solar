// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Base/Layer.h"

namespace Solar {

class ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer();

  virtual void OnAttach() override;
  virtual void OnDetech() override;
  virtual void OnEvent(Event& e) override;

  void Begin();
  void End();

  void SetBlockEvents(bool block) { block_events_ = block; }

  void SetDarkThemeColors();

 private:
  bool block_events_ = true;
  float time_;
};

}  // namespace Solar
