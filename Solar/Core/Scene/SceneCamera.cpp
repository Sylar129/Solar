// Copyright (c) 2024 Sylar129

#include "Core/Scene/SceneCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Solar {

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

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
  aspect_ratio_ = (float)width / (float)height;
  RecalculateProjection();
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

}  // namespace Solar
