// Copyright (c) 2024 Sylar129

#pragma once

#include <array>
#include <string_view>

#include "core/base/log.h"
#include "core/base/sundry.h"
#include "core/renderer/camera.h"
#include "glm/glm.hpp"

namespace solar {

class SceneCamera : public Camera {
 public:
  enum class ProjectionType { kPerspective = 0, kOrthographic };

  SceneCamera();
  virtual ~SceneCamera() = default;

  static constexpr auto GetTypeArray() {
    constexpr std::array kTypes = {SceneCamera::ProjectionType::kPerspective,
                                   SceneCamera::ProjectionType::kOrthographic};
    return kTypes;
  }

  void SetOrthoGraphic(float size, float near_clip, float far_clip);
  void SetPerspective(float vertical_fov, float near_clip, float far_clip);

  // TODO(sylar): set to float
  void SetViewportSize(uint32_t width, uint32_t height);

  float GetPerspectiveVerticalFOV() const { return perspective_fov_; }
  void SetPerspectiveVerticalFOV(float vertical_fov) {
    perspective_fov_ = vertical_fov;
    RecalculateProjection();
  }
  float GetPerspectiveNearClip() const { return perspective_near_; }
  void SetPerspectiveNearClip(float near_clip) {
    perspective_near_ = near_clip;
    RecalculateProjection();
  }
  float GetPerspectiveFarClip() const { return perspective_far_; }
  void SetPerspectiveFarClip(float far_clip) {
    perspective_far_ = far_clip;
    RecalculateProjection();
  }

  float GetOrthographicSize() const { return orthographic_size_; }
  void SetOrthographicSize(float size) {
    orthographic_size_ = size;
    RecalculateProjection();
  }
  float GetOrthographicNearClip() const { return orthographic_near_; }
  void SetOrthographicNearClip(float near_clip) {
    orthographic_near_ = near_clip;
    RecalculateProjection();
  }
  float GetOrthographicFarClip() const { return orthographic_far_; }
  void SetOrthographicFarClip(float far_clip) {
    orthographic_far_ = far_clip;
    RecalculateProjection();
  }

  ProjectionType GetProjectionType() const { return projection_type_; }
  void SetProjectionType(ProjectionType type) { projection_type_ = type; }

 private:
  void RecalculateProjection();

  // Dealing with 2D first right now, TODO: change to Perspective
  ProjectionType projection_type_ = ProjectionType::kOrthographic;

  // Perspective
  float perspective_fov_ = glm::radians(45.0f);
  float perspective_near_ = 0.01f;
  float perspective_far_ = 1000.0f;

  // Orthographic
  float orthographic_size_ = 10.0f;
  float orthographic_near_ = -1.0f;
  float orthographic_far_ = 1.0f;

  float aspect_ratio_ = 0.0f;
};

constexpr std::string_view ToString(SceneCamera::ProjectionType type) {
  switch (type) {
    case SceneCamera::ProjectionType::kPerspective:
      return "Perspective";
    case SceneCamera::ProjectionType::kOrthographic:
      return "Orthographic";
  }

  SOLAR_ASSERT(false, "UNREACHABLE");
}

}  // namespace solar
