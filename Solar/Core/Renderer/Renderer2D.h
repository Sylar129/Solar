#pragma once

#include "Core/Renderer/OrthographicCamera.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/SubTexture2D.h"
#include "Core/Renderer/Camera.h"
#include "Core/Renderer/EditorCamera.h"
#include "Core/Scene/Components.h"

namespace Solar {

class Renderer2D {
public:
    Renderer2D() = delete;
    ~Renderer2D() = delete;

    static void Init();
    static void ShutDown();

    static void BeginScene(const Camera& camera, const glm::mat4 transform);
    static void BeginScene(const EditorCamera& camera);
    static void BeginScene(const OrthographicCamera& camera); // TODO: Remove
    static void EndScene();
    static void Flush();

    // Primitives
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                         const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                         const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                         const Ref<Texture2D>& texture,
                         float tilingFactor = 1.0f,
                         const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                         const Ref<Texture2D>& texture,
                         float tilingFactor = 1.0f,
                         const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                         const Ref<SubTexture2D>& subTexture,
                         float tilingFactor = 1.0f,
                         const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                         const Ref<SubTexture2D>& subTexture,
                         float tilingFactor = 1.0f,
                         const glm::vec4& tintColor = glm::vec4(1.0f));

    static void DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                         int entityID = -1);
    static void DrawQuad(const glm::mat4& transform,
                         const Ref<Texture2D>& texture,
                         float tilingFactor = 1.0f,
                         const glm::vec4& tintColor = glm::vec4(1.0f),
                         int entityID = -1);

    // Rotation is in radians
    static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size,
                               float rotation, const glm::vec4& color);
    static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size,
                               float rotation, const glm::vec4& color);
    static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size,
                               float rotation, const Ref<Texture2D>& texture,
                               float tilingFactor = 1.0f,
                               const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size,
                               float rotation, const Ref<Texture2D>& texture,
                               float tilingFactor = 1.0f,
                               const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size,
                               float rotation,
                               const Ref<SubTexture2D>& subTexture,
                               float tilingFactor = 1.0f,
                               const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size,
                               float rotation,
                               const Ref<SubTexture2D>& subTexture,
                               float tilingFactor = 1.0f,
                               const glm::vec4& tintColor = glm::vec4(1.0f));

    static void DrawSprite(const glm::mat4& transform,
                           SpriteRendererComponent& src, int entityID);

    // Stats
    struct Statistics {
        uint32_t DrawCalls = 0;
        uint32_t QuadCount = 0;

        uint32_t GetTotalVertexCount() { return QuadCount * 4; }
        uint32_t GetTotalIndexCount() { return QuadCount * 6; }
    };
    static void ResetStats();
    static Statistics GetStats();

private:
    static void StartBatch();
    static void NextBatch();
};

} // namespace Solar
