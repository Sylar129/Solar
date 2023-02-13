#pragma once

#include "Solar/Core/Core.h"
#include "Solar/Core/Timestep.h"
#include "Solar/Events/Event.h"

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

} // namespace Solar
