// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "core/base/timestep.h"
#include "core/events/event.h"

namespace solar {

class Layer {
 public:
  explicit Layer(const std::string& name = "Layer");
  virtual ~Layer();

  virtual void OnAttach() {}
  virtual void OnDetech() {}
  virtual void OnUpdate(TimeStep& ts) {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Event& event) {}

  std::string GetName() const { return debug_name_; }

 protected:
  std::string debug_name_;
};

}  // namespace solar