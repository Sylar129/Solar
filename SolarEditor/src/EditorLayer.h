#pragma once

#include <Solar.h>

namespace Solar {
    class EditorLayer : public Layer {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetech() override;

        virtual void OnUpdate(TimeStep& ts) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;
    private:
        OrthographicCameraController m_CameraController;

        Ref<Shader> m_FlatColorShader;
        Ref<VertexArray> m_SquareVA;
        Ref<Texture2D> m_BoardTexture;

        Ref<Framebuffer> m_Framebuffer;

        glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
    };
}
