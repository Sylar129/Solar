#pragma once

#include "Core/Base/Layer.h"

#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"

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

    void SetBlockEvents(bool block) { m_BlockEvents = block; }

    void SetDarkThemeColors();

private:
    bool m_BlockEvents = true;
    float m_Time;
};

} // namespace Solar
