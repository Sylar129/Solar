// Copyright (c) 2024 Sylar129

#include <GLFW/glfw3.h>

#include "Core/Base/Application.h"
#include "Core/Base/Input.h"

namespace Solar {

bool Input::IsKeyPressed(KeyCode keycode) {
  auto *window = static_cast<GLFWwindow *>(
      Application::Get().GetWindow().GetNativeWindow());
  auto state = glfwGetKey(window, static_cast<uint16_t>(keycode));

  return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::IsMouseButtonPressed(MouseCode button) {
  auto *window = static_cast<GLFWwindow *>(
      Application::Get().GetWindow().GetNativeWindow());
  auto state = glfwGetMouseButton(window, static_cast<uint16_t>(button));

  return state == GLFW_PRESS;
}
std::pair<float, float> Input::GetMousePosition() {
  auto *window = static_cast<GLFWwindow *>(
      Application::Get().GetWindow().GetNativeWindow());
  double xpos;
  double ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  return {(float)xpos, (float)ypos};
}
float Input::GetMouseX() {
  auto [x, y] = GetMousePosition();
  return x;
}
float Input::GetMouseY() {
  auto [x, y] = GetMousePosition();
  return y;
}

}  // namespace Solar
