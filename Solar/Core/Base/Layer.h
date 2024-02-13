#pragma once

#include <string>

#include "Core/Base/Timestep.h"
#include "Core/Events/Event.h"

namespace Solar {

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

}  // namespace Solar
