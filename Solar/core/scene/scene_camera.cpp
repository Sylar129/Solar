// Copyright (c) 2024 Sylar129

#include "core/scene/scene_camera.h"

#include "glm/ext/matrix_clip_space.hpp"

namespace solar {

SceneCamera::SceneCamera() { RecalculateProjection(); }

void SceneCamera::SetOrthoGraphic(float size, float near_clip, float far_clip) {
  projection_type_ = ProjectionType::kOrthographic;
  orthographic_size_ = size;
  orthographic_near_ = near_clip;
  orthographic_far_ = far_clip;
  RecalculateProjection();
}

void SceneCamera::SetPerspective(float vertical_fov, float near_clip,
                                 float far_clip) {
  projection_type_ = ProjectionType::kPerspective;
  perspective_fov_ = vertical_fov;
  perspective_near_ = near_clip;
  perspective_far_ = far_clip;
  RecalculateProjection();
}

void SceneCamera::SetViewportSize(const Size& viewport_size) {
  aspect_ratio_ = viewport_size.width / viewport_size.height;
  RecalculateProjection();
}

float SceneCamera::GetPerspectiveVerticalFOV() const {
  return perspective_fov_;
}
void SceneCamera::SetPerspectiveVerticalFOV(float vertical_fov) {
  perspective_fov_ = vertical_fov;
  RecalculateProjection();
}

float SceneCamera::GetPerspectiveNearClip() const { return perspective_near_; }
void SceneCamera::SetPerspectiveNearClip(float near_clip) {
  perspective_near_ = near_clip;
  RecalculateProjection();
}

float SceneCamera::GetPerspectiveFarClip() const { return perspective_far_; }
void SceneCamera::SetPerspectiveFarClip(float far_clip) {
  perspective_far_ = far_clip;
  RecalculateProjection();
}

float SceneCamera::GetOrthographicSize() const { return orthographic_size_; }
void SceneCamera::SetOrthographicSize(float size) {
  orthographic_size_ = size;
  RecalculateProjection();
}

float SceneCamera::GetOrthographicNearClip() const {
  return orthographic_near_;
}
void SceneCamera::SetOrthographicNearClip(float near_clip) {
  orthographic_near_ = near_clip;
  RecalculateProjection();
}

float SceneCamera::GetOrthographicFarClip() const { return orthographic_far_; }
void SceneCamera::SetOrthographicFarClip(float far_clip) {
  orthographic_far_ = far_clip;
  RecalculateProjection();
}

SceneCamera::ProjectionType SceneCamera::GetProjectionType() const {
  return projection_type_;
}
void SceneCamera::SetProjectionType(SceneCamera::ProjectionType type) {
  projection_type_ = type;
}

void SceneCamera::RecalculateProjection() {
  if (projection_type_ == ProjectionType::kPerspective) {
    projection_ = glm::perspective(perspective_fov_, aspect_ratio_,
                                   perspective_near_, perspective_far_);
  } else {
    float ortho_left = -orthographic_size_ * aspect_ratio_ * 0.5f;
    float ortho_right = orthographic_size_ * aspect_ratio_ * 0.5f;
    float ortho_bottom = -orthographic_size_ * 0.5f;
    float ortho_top = orthographic_size_ * 0.5f;

    projection_ = glm::ortho(ortho_left, ortho_right, ortho_bottom, ortho_top,
                             orthographic_near_, orthographic_far_);
  }
}

}  // namespace solar
