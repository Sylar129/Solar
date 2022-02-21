#pragma once

#include "Solar/Core.h"
#include "Solar/Events/Event.h"

namespace Solar {
    class SOLAR_API Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetech() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };
}


