// Copyright (c) 2024 Sylar129

#pragma once

#include <array>
#include <string_view>

#include "core/debug/assert.h"
#include "core/math/size.h"
#include "core/renderer/camera.h"
#include "glm/trigonometric.hpp"

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

  void SetViewportSize(const Size& viewport_size);

  float GetPerspectiveVerticalFOV() const;
  void SetPerspectiveVerticalFOV(float vertical_fov);

  float GetPerspectiveNearClip() const;
  void SetPerspectiveNearClip(float near_clip);

  float GetPerspectiveFarClip() const;
  void SetPerspectiveFarClip(float far_clip);

  float GetOrthographicSize() const;
  void SetOrthographicSize(float size);

  float GetOrthographicNearClip() const;
  void SetOrthographicNearClip(float near_clip);

  float GetOrthographicFarClip() const;
  void SetOrthographicFarClip(float far_clip);

  ProjectionType GetProjectionType() const;
  void SetProjectionType(ProjectionType type);

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
