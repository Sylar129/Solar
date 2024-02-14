// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "core/scene/scene_camera.h"
#include "core/scene/scriptable_entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace solar {

struct TagComponent {
  std::string tag;

  explicit TagComponent(const std::string& tag = "") : tag(tag) {}
};

struct TransformComponent {
  glm::vec3 translation{0, 0, 0};
  glm::vec3 rotation{0, 0, 0};
  glm::vec3 scale{1, 1, 1};

  explicit TransformComponent(const glm::vec3& translation = {0, 0, 0})
      : translation(translation) {}

  glm::mat4 GetTranform() const {
    return glm::translate(glm::mat4(1), translation) *
           glm::toMat4(glm::quat(rotation)) * glm::scale(glm::mat4(1), scale);
  }
};

struct SpriteRendererComponent {
  glm::vec4 color{1, 1, 1, 1};
};

struct CameraComponent {
  SceneCamera camera;
  // TODO(sylar): Move to the scene
  bool primary = true;
  bool fixed_aspect_ratio = false;
};

struct NativeScriptComponent {
  ScriptableEntity* instance = nullptr;

  ScriptableEntity* (*instantiate_script)() = nullptr;
  void (*destory_script)(NativeScriptComponent*) = nullptr;

  template <typename T>
  void Bind() {
    instantiate_script = []() {
      return static_cast<ScriptableEntity*>(new T());
    };
    destory_script = [](NativeScriptComponent* nsc) {
      delete nsc->instance;
      nsc->instance = nullptr;
    };
  }
};

}  // namespace solar
