// Copyright (c) 2024 Sylar129

#pragma once

#include "glm/mat4x4.hpp"

namespace solar {

class Camera {
 public:
  Camera() = default;
  explicit Camera(const glm::mat4& projection) : projection_(projection) {}
  virtual ~Camera() = default;

  const glm::mat4& GetProjection() const { return projection_; }

 protected:
  glm::mat4 projection_ = glm::mat4(1.0f);
};

}  // namespace solar
