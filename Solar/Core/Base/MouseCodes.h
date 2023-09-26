#pragma once

#include <ostream>

namespace Solar {

enum class MouseCode : uint16_t {
  // From glfw3.h
  Button1 = 0,
  Button2 = 1,
  Button3 = 2,
  Button4 = 3,
  Button5 = 4,
  Button6 = 5,
  Button7 = 6,
  Button8 = 7,

  ButtonLast = Button8,
  ButtonLeft = Button1,
  ButtonRight = Button2,
  ButtonMiddle = Button3
};

inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode) {
  os << static_cast<int32_t>(mouseCode);
  return os;
}

}  // namespace Solar
