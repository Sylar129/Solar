// Copyright (c) 2024 Sylar129

#pragma once

#include <ostream>

namespace Solar {

enum class MouseCode : uint16_t {
  // From glfw3.h
  kButton1 = 0,
  kButton2 = 1,
  kButton3 = 2,
  kButton4 = 3,
  kButton5 = 4,
  kButton6 = 5,
  kButton7 = 6,
  kButton8 = 7,

  kButtonLast = kButton8,
  kButtonLeft = kButton1,
  kButtonRight = kButton2,
  kButtonMiddle = kButton3
};

inline std::ostream& operator<<(std::ostream& os, MouseCode mouse_code) {
  os << static_cast<int32_t>(mouse_code);
  return os;
}

}  // namespace Solar
