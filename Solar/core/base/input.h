// Copyright (c) 2024 Sylar129

#pragma once

#include "core/base/key_codes.h"
#include "core/base/mouse_codes.h"
#include "glm/fwd.hpp"

namespace solar {

class Input {
 public:
  static bool IsKeyPressed(KeyCode keycode);
  static bool IsMouseButtonPressed(MouseCode button);
  static glm::vec2 GetMousePosition();
};

}  // namespace solar
