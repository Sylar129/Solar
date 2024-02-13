#pragma once

#include <Solar.h>

#include "Core/Renderer/EditorCamera.h"
#include "Core/Events/KeyEvent.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Solar {

class EditorLayer : public Layer {
 public:
  EditorLayer();
  virtual ~EditorLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetech() override;

  virtual void OnUpdate(TimeStep& ts) override;
  virtual void OnImGuiRender() override;
  virtual void OnEvent(Event& event) override;

 private:
  bool OnKeyPressed(KeyPressdEvent& e);
  bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

  void NewScene();
  void OpenScene();
  void SaveSceneAs();

  OrthographicCameraController camera_controller_;

  Ref<Shader> flat_color_shader_;
  Ref<VertexArray> square_va_;

  Ref<Framebuffer> framebuffer_;

  // Entity
  Ref<Scene> active_scene_;
  Entity square_entity_;
  Entity camera_entity_;
  Entity second_camera_;
  bool primary_camera_ = true;

  Entity hovered_entity_;

  EditorCamera editor_camera_;

  bool viewport_focused_ = false;
  bool viewport_hovered_ = false;

  glm::vec2 viewport_size_ = {0.0f, 0.0f};
  glm::vec2
      viewport_bounds_[2];  // 0 stands for min bound, 1 stands for max bound

  glm::vec4 square_color_ = {0.2f, 0.3f, 0.8f, 1.0f};

  int gizmo_type_ = -1;

  // Panels
  SceneHierarchyPanel scene_hierarchy_panel_;
};

}  // namespace Solar
