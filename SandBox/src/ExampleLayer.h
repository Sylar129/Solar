#pragma once

#include <Solar.h>

class ExampleLayer : public Solar::Layer {
public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    void OnUpdate(Solar::TimeStep& ts) override;

    void OnImGuiRender() override;

    void OnEvent(Solar::Event& e) override;
private:
    Solar::ShaderLibrary m_ShaderLibrary;
    Solar::Ref<Solar::Shader> m_Shader;
    Solar::Ref<Solar::VertexArray> m_VertexArray;

    Solar::Ref<Solar::Shader> m_SquareShader;
    Solar::Ref<Solar::VertexArray> m_SquareVA;

    Solar::Ref<Solar::Texture2D> m_Texture, m_LogoTexture;

    Solar::OrthographicCameraController m_CameraController;

    glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
};
