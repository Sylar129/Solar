// Copyright (c) 2024 Sylar129

#include "core/renderer/orthographic_camera_controller.h"

#include <algorithm>

#include "core/base/input.h"
#include "core/base/key_codes.h"
#include "core/debug/instrumentor.h"
#include "glm/glm.hpp"

namespace solar {

OrthographicCameraController::OrthographicCameraController(float aspect_ratio,
                                                           bool rotation)
    : aspect_ratio_(aspect_ratio),
      zoom_level_(1.0f),
      bounds_({-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_,
               -zoom_level_, zoom_level_}),
      camera_(bounds_.left, bounds_.right, bounds_.bottom, bounds_.top),
      camera_position_(0.0f, 0.0f, 0.0f),
      camera_translation_speed_(1.0f),
      rotation_(rotation),
      camera_rotation_(0.0f),
      camera_rotation_speed_(1.0f) {}

void OrthographicCameraController::OnUpdate(TimeStep ts) {
  SOLAR_PROFILE_FUNCTION();

  // SOLAR_CORE_TRACE("Rotation: [{0}]", m_CameraRotation);
  if (Input::IsKeyPressed(KeyCode::kA)) {
    camera_position_.x -=
        glm::cos(camera_rotation_) * camera_translation_speed_ * ts;
    camera_position_.y -=
        glm::sin(camera_rotation_) * camera_translation_speed_ * ts;
  }
  if (Input::IsKeyPressed(KeyCode::kD)) {
    camera_position_.x +=
        glm::cos(camera_rotation_) * camera_translation_speed_ * ts;
    camera_position_.y +=
        glm::sin(camera_rotation_) * camera_translation_speed_ * ts;
  }
  if (Input::IsKeyPressed(KeyCode::kW)) {
    camera_position_.x +=
        -glm::sin(camera_rotation_) * camera_translation_speed_ * ts;
    camera_position_.y +=
        glm::cos(camera_rotation_) * camera_translation_speed_ * ts;
  }
  if (Input::IsKeyPressed(KeyCode::kS)) {
    camera_position_.x -=
        -glm::sin(camera_rotation_) * camera_translation_speed_ * ts;
    camera_position_.y -=
        glm::cos(camera_rotation_) * camera_translation_speed_ * ts;
  }

  if (rotation_) {
    if (Input::IsKeyPressed(KeyCode::kQ)) {
      camera_rotation_ += camera_rotation_speed_ * ts;
    }
    if (Input::IsKeyPressed(KeyCode::kE)) {
      camera_rotation_ -= camera_rotation_speed_ * ts;
    }
    camera_.SetRotation(camera_rotation_);
  }

  camera_.SetPosition(camera_position_);
  camera_translation_speed_ = zoom_level_;
}

void OrthographicCameraController::OnEvent(Event& e) {
  SOLAR_PROFILE_FUNCTION();

  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<MouseScrolledEvent>(SOLAR_BIND_EVENT_FN(OnMouseScrolled));
  dispatcher.Dispatch<WindowResizeEvent>(SOLAR_BIND_EVENT_FN(OnWindowResized));
}

void OrthographicCameraController::OnResize(float width, float height) {
  aspect_ratio_ = width / height;
  CalculateView();
}

void OrthographicCameraController::CalculateView() {
  bounds_ = {-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_,
             -zoom_level_, zoom_level_};
  camera_.SetProjection(bounds_.left, bounds_.right, bounds_.bottom,
                        bounds_.top);
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
  SOLAR_PROFILE_FUNCTION();

  zoom_level_ -= e.GetYOffset() * 0.1f;
  zoom_level_ = std::max(zoom_level_, 0.25f);
  CalculateView();
  return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
  SOLAR_PROFILE_FUNCTION();

  OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
  return false;
}

}  // namespace solar
