#pragma once

#include "Core/Base/Timestep.h"
#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Renderer/OrthographicCamera.h"

namespace Solar {

struct OrthographicCameraBounds {
  float Left, Right;
  float Bottom, Top;

  float GetWidth() { return Right - Left; }
  float GetHeight() { return Top - Bottom; }
};

class OrthographicCameraController {
 public:
  OrthographicCameraController(float aspectRatio, bool rotation = false);

  void OnUpdate(TimeStep ts);
  void OnEvent(Event& e);

  void OnResize(float width, float height);

  OrthographicCamera& GetCamera() { return m_Camera; }
  const OrthographicCamera& GetCamera() const { return m_Camera; }

  float GetZoomLevel() const { return m_ZoomLevel; }
  void SetZoomLevel(float level) {
    m_ZoomLevel = level;
    CalculateView();
  }

  const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

 private:
  void CalculateView();

  bool OnMouseScrolled(MouseScrolledEvent& e);
  bool OnWindowResized(WindowResizeEvent& e);

 private:
  float m_AspectRatio;
  float m_ZoomLevel;
  OrthographicCameraBounds m_Bounds;
  OrthographicCamera m_Camera;

  glm::vec3 m_CameraPosition;
  float m_CameraTranslationSpeed;

  bool m_Rotation;
  float m_CameraRotation;  // radians
  float m_CameraRotationSpeed;
};

}  // namespace Solar
