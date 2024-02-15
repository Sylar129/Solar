// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "core/base/timestep.h"
#include "core/math/size.h"
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

  void OnUpdateRuntime(const TimeStep& ts);
  void OnUpdateEditor(const TimeStep& ts, EditorCamera& camera);
  void OnViewportResize(const Size& size);

  Entity GetPrimaryCameraEntity();

 private:
  template <typename T>
  void OnComponentAdded(Entity entity, T& component);

  entt::registry registry_;
  Size viewport_size_;

  friend class Entity;
  friend class SceneSerializer;
  friend class SceneHierarchyPanel;
};

}  // namespace solar
