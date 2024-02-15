// Copyright (c) 2024 Sylar129

#pragma once

#include "core/scene/entity.h"
#include "core/scene/scene.h"
#include "core/utils/misc.h"

namespace solar {

class SceneHierarchyPanel {
 public:
  SceneHierarchyPanel() = default;
  explicit SceneHierarchyPanel(const Ref<Scene>& context);

  void SetContext(const Ref<Scene>& context);

  void OnImGuiRender();

  Entity GetSelectedEntity() const { return selection_context_; }
  void SetSelectedEntity(Entity entity);

 private:
  void DrawEntityNode(Entity entity);
  void DrawComponents(Entity entity);

  Ref<Scene> context_;
  Entity selection_context_;
};

}  // namespace solar
