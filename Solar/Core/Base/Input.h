// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Base/KeyCodes.h"
#include "Core/Base/MouseCodes.h"

namespace Solar {

class Input {
 public:
  static bool IsKeyPressed(KeyCode keycode);
  static bool IsMouseButtonPressed(MouseCode button);
  static std::pair<float, float> GetMousePosition();
  static float GetMouseX();
  static float GetMouseY();
};

}  // namespace Solar
