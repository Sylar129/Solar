// Copyright (c) 2024 Sylar129

#include "core/renderer/editor_camera.h"

#include <algorithm>
#include <utility>

#include "core/base/input.h"
#include "core/base/key_codes.h"
#include "core/base/mouse_codes.h"
#include "glm/gtx/quaternion.hpp"

namespace solar {

EditorCamera::EditorCamera(float fov, float aspect_ratio, float near_clip,
                           float far_clip)
    : fov_(fov),
      aspect_ratio_(aspect_ratio),
      near_clip_(near_clip),
      far_clip_(far_clip),
      Camera(glm::perspective(glm::radians(fov), aspect_ratio, near_clip,
                              far_clip)) {
  UpdateView();
}

void EditorCamera::OnUpdate(TimeStep& ts) {
  if (Input::IsKeyPressed(KeyCode::kLeftAlt)) {
    const glm::vec2& mouse{Input::GetMouseX(), Input::GetMouseY()};
    glm::vec2 delta = (mouse - initial_mouse_position_) * 0.003f;
    initial_mouse_position_ = mouse;

    if (Input::IsMouseButtonPressed(MouseCode::kButtonMiddle)) {
      MousePan(delta);
    } else if (Input::IsMouseButtonPressed(MouseCode::kButtonLeft)) {
      MouseRotate(delta);
    } else if (Input::IsMouseButtonPressed(MouseCode::kButtonRight)) {
      MouseZoom(delta.y);
    }
  }

  UpdateView();
}

void EditorCamera::OnEvent(Event& e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch(this, &EditorCamera::OnMouseScroll);
}

float EditorCamera::GetDistance() const { return distance_; }

void EditorCamera::SetDistance(float distance) { distance_ = distance; }

void EditorCamera::SetViewportSize(float width, float height) {
  viewport_width_ = width;
  viewport_height_ = height;
  UpdateProjection();
}

const glm::mat4& EditorCamera::GetViewMatrix() const { return view_matrix_; }

const glm::vec3& EditorCamera::GetPosition() const { return position_; }

float EditorCamera::GetPitch() const { return pitch_; }

float EditorCamera::GetYaw() const { return yaw_; }

glm::mat4 EditorCamera::CalcViewProjection() const {
  return projection_ * view_matrix_;
}

glm::vec3 EditorCamera::CalcUpDirection() const {
  return glm::rotate(CalcOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 EditorCamera::CalcRightDirection() const {
  return glm::rotate(CalcOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 EditorCamera::CalcForwardDirection() const {
  return glm::rotate(CalcOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::quat EditorCamera::CalcOrientation() const {
  return glm::quat(glm::vec3(-pitch_, -yaw_, 0.0f));
}

void EditorCamera::UpdateProjection() {
  aspect_ratio_ = viewport_width_ / viewport_height_;
  projection_ = glm::perspective(glm::radians(fov_), aspect_ratio_, near_clip_,
                                 far_clip_);
}

void EditorCamera::UpdateView() {
  // m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
  position_ = CalculatePosition();

  glm::quat orientation = CalcOrientation();
  view_matrix_ =
      glm::translate(glm::mat4(1.0f), position_) * glm::toMat4(orientation);
  view_matrix_ = glm::inverse(view_matrix_);
}

bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e) {
  float delta = e.GetYOffset() * 0.1f;
  MouseZoom(delta);
  UpdateView();
  return false;
}

void EditorCamera::MousePan(const glm::vec2& delta) {
  auto [xSpeed, ySpeed] = PanSpeed();
  focal_point_ += -CalcRightDirection() * delta.x * xSpeed * distance_;
  focal_point_ += CalcUpDirection() * delta.y * ySpeed * distance_;
}

void EditorCamera::MouseRotate(const glm::vec2& delta) {
  float yaw_sign = CalcUpDirection().y < 0 ? -1.0f : 1.0f;
  yaw_ += yaw_sign * delta.x * RotationSpeed();
  pitch_ += delta.y * RotationSpeed();
}

void EditorCamera::MouseZoom(float delta) {
  distance_ -= delta * ZoomSpeed();
  if (distance_ < 1.0f) {
    focal_point_ += CalcForwardDirection();
    distance_ = 1.0f;
  }
}

glm::vec3 EditorCamera::CalculatePosition() const {
  return focal_point_ - CalcForwardDirection() * distance_;
}

std::pair<float, float> EditorCamera::PanSpeed() const {
  float x = std::min(viewport_width_ / 1000.0f, 2.4f);  // max = 2.4f
  float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

  float y = std::min(viewport_height_ / 1000.0f, 2.4f);  // max = 2.4f
  float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

  return {x_factor, y_factor};
}

float EditorCamera::RotationSpeed() const { return 0.8f; }

float EditorCamera::ZoomSpeed() const {
  float distance = distance_ * 0.2f;
  distance = std::max(distance, 0.0f);
  float speed = distance * distance;
  speed = std::min(speed, 100.0f);  // max speed = 100
  return speed;
}

}  // namespace solar
