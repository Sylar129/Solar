// Copyright (c) 2024 Sylar129

#include "core/base/input.h"

#include "GLFW/glfw3.h"
#include "core/base/application.h"
#include "glm/fwd.hpp"

namespace solar {

bool Input::IsKeyPressed(KeyCode keycode) {
  auto *window = static_cast<GLFWwindow *>(
      Application::Get().GetWindow().GetNativeWindow());
  auto state = glfwGetKey(window, static_cast<int>(keycode));

  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseCode button) {
  auto *window = static_cast<GLFWwindow *>(
      Application::Get().GetWindow().GetNativeWindow());
  auto state = glfwGetMouseButton(window, static_cast<int>(button));

  return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition() {
  auto *window = static_cast<GLFWwindow *>(
      Application::Get().GetWindow().GetNativeWindow());
  double xpos;
  double ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  return {static_cast<float>(xpos), static_cast<float>(ypos)};
}

}  // namespace solar
