// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "core/base/timestep.h"
#include "core/events/event.h"

namespace solar {

class Layer {
 public:
  explicit Layer(const std::string& debug_name);
  virtual ~Layer();

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(const TimeStep& ts) {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Event& event) {}

  std::string GetDebugName() const;

 protected:
  std::string debug_name_;
};

}  // namespace solar
