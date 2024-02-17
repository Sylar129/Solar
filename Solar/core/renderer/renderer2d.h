// Copyright (c) 2024 Sylar129

#pragma once

#include "core/renderer/camera.h"
#include "core/renderer/editor_camera.h"
#include "core/renderer/orthographic_camera.h"
#include "core/renderer/subtexture2d.h"
#include "core/renderer/texture.h"
#include "core/scene/components.h"

namespace solar {

class Statistics;

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
  static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                       const glm::vec4& color);
  static void DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                       int entity_id = -1);

  static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                       const glm::vec4& color, const Ref<Texture2D>& texture,
                       float tiling_factor);
  static void DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                       const Ref<Texture2D>& texture, float tiling_factor,
                       int entity_id = -1);

  static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
                       const glm::vec4& color,
                       const Ref<SubTexture2D>& sub_texture,
                       float tiling_factor);
  static void DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                       const Ref<SubTexture2D>& sub_texture,
                       float tiling_factor);

  // Rotation is in radians
  static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation, const glm::vec4& color);

  static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation, const glm::vec4& color,
                             const Ref<Texture2D>& texture,
                             float tiling_factor);

  static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size,
                             float rotation, const glm::vec4& color,
                             const Ref<SubTexture2D>& sub_texture,
                             float tiling_factor);

  static void DrawSprite(const TransformComponent& transform,
                         const SpriteRendererComponent& src, int entity_id);

  static void ResetStats();
  static Statistics GetStats();

 private:
  static void StartBatch();
  static void NextBatch();
};

}  // namespace solar
