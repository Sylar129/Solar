// Copyright (c) 2024 Sylar129

#include "core/renderer/orthographic_camera.h"

#include "core/debug/instrumentor.h"
#include "glm/gtc/matrix_transform.hpp"

namespace solar {

OrthographicCamera::OrthographicCamera(float left, float right, float bottom,
                                       float top)
    : projection_matrix_(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
      view_matrix_(1.0f),
      view_projection_matrix_(projection_matrix_ * view_matrix_),
      position_({0.0f, 0.0f, 0.0f}),
      rotation_(0.0f) {}

OrthographicCamera::~OrthographicCamera() {}

void OrthographicCamera::SetProjection(float left, float right, float bottom,
                                       float top) {
  SOLAR_PROFILE_FUNCTION();

  projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
  view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

const glm::vec3& OrthographicCamera::GetPosition() const { return position_; }

void OrthographicCamera::SetPosition(const glm::vec3& position) {
  position_ = position;
  RecalculateViewMatrix();
}

float OrthographicCamera::GetRotation() const { return rotation_; }

void OrthographicCamera::SetRotation(float rotation) {
  rotation_ = rotation;
  RecalculateViewMatrix();
}

const glm::mat4& OrthographicCamera::GetProjectionMatrix() const {
  return projection_matrix_;
}

const glm::mat4& OrthographicCamera::GetViewMatrix() const {
  return view_matrix_;
}

const glm::mat4& OrthographicCamera::GetViewProjectionMatrix() const {
  return view_projection_matrix_;
}

void OrthographicCamera::RecalculateViewMatrix() {
  SOLAR_PROFILE_FUNCTION();

  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f), position_) *
      glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0, 0, 1));

  view_matrix_ = glm::inverse(transform);
  view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

}  // namespace solar
