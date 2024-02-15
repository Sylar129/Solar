// Copyright (c) 2024 Sylar129

#pragma once

#include "core/base/timestep.h"
#include "core/events/mouse_event.h"
#include "core/math/size.h"
#include "core/renderer/camera.h"
#include "glm/fwd.hpp"

namespace solar {

class EditorCamera : public Camera {
 public:
  EditorCamera() = default;
  EditorCamera(float fov, float aspect_ratio, float near_clip, float far_clip);

  void OnUpdate(const TimeStep& ts);
  void OnEvent(Event& e);

  float GetDistance() const;
  void SetDistance(float distance);

  void SetViewportSize(const Size& viewport_size);

  const glm::mat4& GetViewMatrix() const;
  const glm::vec3& GetPosition() const;
  float GetPitch() const;
  float GetYaw() const;

  glm::mat4 CalcViewProjection() const;
  glm::vec3 CalcUpDirection() const;
  glm::vec3 CalcRightDirection() const;
  glm::vec3 CalcForwardDirection() const;
  glm::quat CalcOrientation() const;

 private:
  void UpdateProjection();
  void UpdateView();

  bool OnMouseScroll(MouseScrolledEvent& e);

  void MousePan(const glm::vec2& delta);
  void MouseRotate(const glm::vec2& delta);
  void MouseZoom(float delta);

  glm::vec3 CalculatePosition() const;

  float RotationSpeed() const;
  float ZoomSpeed() const;

  float fov_ = 45.0f;
  float aspect_ratio_ = 1.778f;
  float near_clip_ = 0.1f;
  float far_clip_ = 1000.0f;

  glm::mat4 view_matrix_;
  glm::vec3 position_ = {0.0f, 0.0f, 0.0f};
  glm::vec3 focal_point_ = {0.0f, 0.0f, 0.0f};

  glm::vec2 initial_mouse_position_ = {0.0f, 0.0f};
  float distance_ = 10.0f;
  float pitch_ = 0.0f;
  float yaw_ = 0.0f;
  Size viewport_size_;
};

}  // namespace solar
