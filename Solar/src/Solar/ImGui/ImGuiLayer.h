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
        virtual void OnEvent(Event& e) override;

        void Begin();
        void End();

        void SetBlockEvents(bool block) {
            m_BlockEvents = block;
        }

        void SetDarkThemeColors();
    private:
        bool m_BlockEvents = true;
        float m_Time;
    };
}
