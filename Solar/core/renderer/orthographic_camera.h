// Copyright (c) 2024 Sylar129

#pragma once

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace solar {

class OrthographicCamera {
 public:
  OrthographicCamera(float left, float right, float bottom, float top);
  ~OrthographicCamera();

  void SetProjection(float left, float right, float bottom, float top);

  const glm::vec3& GetPosition() const;
  void SetPosition(const glm::vec3& position);

  float GetRotation() const;
  void SetRotation(float rotation);

  const glm::mat4& GetProjectionMatrix() const;
  const glm::mat4& GetViewMatrix() const;
  const glm::mat4& GetViewProjectionMatrix() const;

 private:
  void RecalculateViewMatrix();

  glm::mat4 projection_matrix_;
  glm::mat4 view_matrix_;
  glm::mat4 view_projection_matrix_;

  glm::vec3 position_;
  float rotation_;
};

}  // namespace solar
