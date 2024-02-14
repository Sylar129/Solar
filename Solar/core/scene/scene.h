// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "core/base/timestep.h"
#include "core/renderer/editor_camera.h"
#include "entt/entt.hpp"

namespace solar {

class Entity;

class Scene {
 public:
  Scene();
  ~Scene();

  Entity CreateEntity(const std::string& name);
  void DestoryEntity(Entity entity);

  void OnUpdateRuntime(TimeStep& ts);
  void OnUpdateEditor(TimeStep& ts, EditorCamera& camera);
  void OnViewportResize(uint32_t width, uint32_t height);

  Entity GetPrimaryCameraEntity();

 private:
  template <typename T>
  void OnComponentAdded(Entity entity, T& component);

  entt::registry registry_;
  uint32_t viewport_width_ = 0;
  uint32_t viewport_height_ = 0;

  friend class Entity;
  friend class SceneSerializer;
  friend class SceneHierarchyPanel;
};

}  // namespace solar