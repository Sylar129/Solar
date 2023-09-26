#pragma once

#include "Core/Base/Timestep.h"
#include "Core/Renderer/EditorCamera.h"
#include "entt/entt.hpp"

namespace Solar {

class Entity;

class Scene {
 public:
  Scene();
  ~Scene();

  Entity CreateEntity(const std::string& name = std::string());
  void DestoryEntity(Entity entity);

  void OnUpdateRuntime(TimeStep& ts);
  void OnUpdateEditor(TimeStep& ts, EditorCamera& camera);
  void OnViewportResize(uint32_t width, uint32_t height);

  Entity GetPrimaryCameraEntity();

 private:
  template <typename T>
  void OnComponentAdded(Entity entity, T& component);

 private:
  entt::registry m_Registry;
  uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

  friend class Entity;
  friend class SceneSerializer;
  friend class SceneHierarchyPanel;
};

}  // namespace Solar
