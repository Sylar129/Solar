#pragma once

#include "Core/Scene/SceneCamera.h"
#include "Core/Scene/ScriptableEntity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Solar {

struct TagComponent {
  std::string Tag;

  TagComponent() = default;
  TagComponent(const TagComponent&) = default;
  TagComponent(const std::string& tag) : Tag(tag) {}
};

struct TransformComponent {
  glm::vec3 Translation{0.0f, 0.0f, 0.0f};
  glm::vec3 Rotation{0.0f, 0.0f, 0.0f};
  glm::vec3 Scale{1.0f, 1.0f, 1.0f};

  TransformComponent() = default;
  TransformComponent(const TransformComponent&) = default;
  TransformComponent(const glm::vec3& translation) : Translation(translation) {}

  glm::mat4 GetTranform() const {
    glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

    return glm::translate(glm::mat4(1.0f), Translation) * rotation *
           glm::scale(glm::mat4(1.0f), Scale);
  }
};

struct SpriteRendererComponent {
  glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};

  SpriteRendererComponent() = default;
  SpriteRendererComponent(const SpriteRendererComponent&) = default;
  SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
};

struct CameraComponent {
  SceneCamera Camera;
  bool Primary = true;  // TODO: Move to the scene
  bool FixedAspectRatio = false;

  CameraComponent() = default;
  CameraComponent(const CameraComponent&) = default;
};

struct NativeScriptComponent {
  ScriptableEntity* Instance = nullptr;

  ScriptableEntity* (*InstantiateScript)() = nullptr;
  void (*DestoryScript)(NativeScriptComponent*) = nullptr;

  template <typename T>
  void Bind() {
    InstantiateScript = []() {
      return static_cast<ScriptableEntity*>(new T());
    };
    DestoryScript = [](NativeScriptComponent* nsc) {
      delete nsc->Instance;
      nsc->Instance = nullptr;
    };
  }
};

}  // namespace Solar
