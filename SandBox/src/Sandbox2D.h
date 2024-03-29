#pragma once

#include <Solar.h>

class Sandbox2D : public Solar::Layer {
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetech() override;

    virtual void OnUpdate(Solar::TimeStep& ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Solar::Event& event) override;
private:
    Solar::OrthographicCameraController m_CameraController;

    Solar::Ref<Solar::Shader> m_FlatColorShader;
    Solar::Ref<Solar::VertexArray> m_SquareVA;
    Solar::Ref<Solar::Texture2D> m_BoardTexture;

    Solar::Ref<Solar::Texture2D> m_SpriteSheet;
    Solar::Ref<Solar::SubTexture2D> m_TextureStairs;
    Solar::Ref<Solar::SubTexture2D> m_TextureGreednLand;
    Solar::Ref<Solar::SubTexture2D> m_TextureTree;

    Solar::Ref<Solar::Framebuffer> m_Framebuffer;

    glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};

    uint32_t m_MapWidth, m_MapHeight;
    std::unordered_map<char, Solar::Ref<Solar::SubTexture2D>> s_TextureMap;
};