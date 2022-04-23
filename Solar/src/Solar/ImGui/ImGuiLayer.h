#pragma once

#include "Solar/Core/Layer.h"
#include "Solar/Events/ApplicationEvent.h"
#include "Solar/Events/KeyEvent.h"
#include "Solar/Events/MouseEvent.h"

namespace Solar {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetech() override;

        void Begin();
        void End();
    private:
        float m_Time;
    };
}
