// Copyright (c) 2024 Sylar129

#pragma once

#include "core/base/timestep.h"
#include "core/events/application_event.h"
#include "core/events/mouse_event.h"
#include "core/renderer/orthographic_camera.h"

namespace solar {

struct OrthographicCameraBounds {
  float left, right;
  float bottom, top;

  float GetWidth() const { return right - left; }
  float GetHeight() const { return top - bottom; }
};

class OrthographicCameraController {
 public:
  explicit OrthographicCameraController(float aspect_ratio,
                                        bool rotation = false);

  void OnUpdate(TimeStep ts);
  void OnEvent(Event& e);

  void OnResize(float width, float height);

  OrthographicCamera& GetCamera() { return camera_; }
  const OrthographicCamera& GetCamera() const { return camera_; }

  float GetZoomLevel() const { return zoom_level_; }
  void SetZoomLevel(float level) {
    zoom_level_ = level;
    CalculateView();
  }

  const OrthographicCameraBounds& GetBounds() const { return bounds_; }

 private:
  void CalculateView();

  bool OnMouseScrolled(MouseScrolledEvent& e);
  bool OnWindowResized(WindowResizeEvent& e);

  float aspect_ratio_;
  float zoom_level_;
  OrthographicCameraBounds bounds_;
  OrthographicCamera camera_;

  glm::vec3 camera_position_;
  float camera_translation_speed_;

  bool rotation_;
  float camera_rotation_;  // radians
  float camera_rotation_speed_;
};

}  // namespace solar
