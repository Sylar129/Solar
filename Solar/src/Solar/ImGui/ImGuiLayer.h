#pragma once

#include "Solar/Layer.h"

namespace Solar {
    class SOLAR_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetech();
        void OnUpdate();
        void OnEvent(Event& event);
    private:
        float m_Time;
    };
}
