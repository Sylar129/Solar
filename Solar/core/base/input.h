// Copyright (c) 2024 Sylar129

#pragma once

// TODO(sylar): remove
#include <utility>

#include "core/base/key_codes.h"
#include "core/base/mouse_codes.h"

namespace solar {

// TODO(syalr): remove static
class Input {
 public:
  static bool IsKeyPressed(KeyCode keycode);
  static bool IsMouseButtonPressed(MouseCode button);
  static std::pair<float, float> GetMousePosition();
  static float GetMouseX();
  static float GetMouseY();
};

}  // namespace solar
