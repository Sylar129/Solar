// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Renderer/Camera.h"
#include "Core/Renderer/EditorCamera.h"
#include "Core/Renderer/OrthographicCamera.h"
#include "Core/Renderer/SubTexture2D.h"
#include "Core/Renderer/Texture.h"
#include "Core/Scene/Components.h"

namespace solar {

class Renderer2D {
 public:
  Renderer2D() = delete;
  ~Renderer2D() = delete;

  static void Init();
  static void ShutDown();

  static void BeginScene(const Camera& camera, const glm::mat4& transform);
  static void BeginScene(const EditorCamera& camera);
  // TODO(sylar): Remove
  static void BeginScene(const OrthographicCamera& camera);
  static void EndScene();
  static void Flush();

  // Primitives
  static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                       const glm::vec4& color);
  static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                       const glm::vec4& color);
  static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                       const Ref<Texture2D>& texture,
                       float tiling_factor = 1.0f,
                       const glm::vec4& tint_color = glm::vec4(1.0f));
  static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                       const Ref<Texture2D>& texture,
                       float tiling_factor = 1.0f,
                       const glm::vec4& tint_color = glm::vec4(1.0f));
  static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                       const Ref<SubTexture2D>& sub_texture,
                       float tiling_factor = 1.0f,
                       const glm::vec4& tint_color = glm::vec4(1.0f));
  static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                       const Ref<SubTexture2D>& sub_texture,
                       float tiling_factor = 1.0f,
                       const glm::vec4& tint_color = glm::vec4(1.0f));

  static void DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                       int entity_id = -1);
  static void DrawQuad(const glm::mat4& transform,
                       const Ref<Texture2D>& texture,
                       float tiling_factor = 1.0f,
                       const glm::vec4& tint_color = glm::vec4(1.0f),
                       int entity_id = -1);

  // Rotation is in radians
  static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size,
                             float rotation, const glm::vec4& color);
  static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation, const glm::vec4& color);
  static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size,
                             float rotation, const Ref<Texture2D>& texture,
                             float tiling_factor = 1.0f,
                             const glm::vec4& tint_color = glm::vec4(1.0f));
  static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation, const Ref<Texture2D>& texture,
                             float tiling_factor = 1.0f,
                             const glm::vec4& tint_color = glm::vec4(1.0f));
  static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size,
                             float rotation,
                             const Ref<SubTexture2D>& sub_texture,
                             float tiling_factor = 1.0f,
                             const glm::vec4& tint_color = glm::vec4(1.0f));
  static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation,
                             const Ref<SubTexture2D>& sub_texture,
                             float tiling_factor = 1.0f,
                             const glm::vec4& tint_color = glm::vec4(1.0f));

  static void DrawSprite(const glm::mat4& transform,
                         const SpriteRendererComponent& src, int entity_id);

  // Stats
  struct Statistics {
    uint32_t draw_calls = 0;
    uint32_t quad_count = 0;

    uint32_t GetTotalVertexCount() const { return quad_count * 4; }
    uint32_t GetTotalIndexCount() const { return quad_count * 6; }
  };
  static void ResetStats();
  static Statistics GetStats();

 private:
  static void StartBatch();
  static void NextBatch();
};

}  // namespace solar
