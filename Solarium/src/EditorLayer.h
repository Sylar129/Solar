#pragma once

#include <Solar.h>

#include "Core/Renderer/EditorCamera.h"
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

 private:
  OrthographicCameraController m_CameraController;

  Ref<Shader> m_FlatColorShader;
  Ref<VertexArray> m_SquareVA;

  Ref<Framebuffer> m_Framebuffer;

  // Entity
  Ref<Scene> m_ActiveScene;
  Entity m_SquareEntity;
  Entity m_CameraEntity;
  Entity m_SecondCamera;
  bool m_PrimaryCamera = true;

  Entity m_HoveredEntity;

  EditorCamera m_EditorCamera;

  bool m_ViewportFocused = false;
  bool m_ViewportHovered = false;

  glm::vec2 m_ViewportSize = {0.0f, 0.0f};
  glm::vec2
      m_ViewportBounds[2];  // 0 stands for min bound, 1 stands for max bound

  glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};

  int m_GizmoType = -1;

  // Panels
  SceneHierarchyPanel m_SceneHierarchyPanel;
};

}  // namespace Solar
