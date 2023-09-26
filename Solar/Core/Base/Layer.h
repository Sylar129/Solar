#pragma once

#include <string>

#include "Core/Base/Sundry.h"
#include "Core/Base/Timestep.h"
#include "Core/Events/Event.h"

namespace Solar {

class Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer();

  virtual void OnAttach() {}
  virtual void OnDetech() {}
  virtual void OnUpdate(TimeStep& ts) {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Event& event) {}

  inline const std::string GetName() const { return m_DebugName; }

 protected:
  std::string m_DebugName;
};

}  // namespace Solar
