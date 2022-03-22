#pragma once

namespace Solar {
    enum class MouseCode : uint16_t {
        // From glfw3.h
        Button1         = 0,
        Button2         = 1,
        Button3         = 2,
        Button4         = 3,
        Button5         = 4,
        Button6         = 5,
        Button7         = 6,
        Button8         = 7,

        ButtonLast      = Button8,
        ButtonLeft      = Button1,
        ButtonRight     = Button2,
        ButtonMiddle    = Button3
    };

    inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode) {
        os << static_cast<int32_t>(mouseCode);
        return os;
    }
}


// From glfw3.h
#define SOLAR_MOUSE_BUTTON_1        ::Solar::Mouse::Button1
#define SOLAR_MOUSE_BUTTON_2        ::Solar::Mouse::Button2
#define SOLAR_MOUSE_BUTTON_3        ::Solar::Mouse::Button3
#define SOLAR_MOUSE_BUTTON_4        ::Solar::Mouse::Button4
#define SOLAR_MOUSE_BUTTON_5        ::Solar::Mouse::Button5
#define SOLAR_MOUSE_BUTTON_6        ::Solar::Mouse::Button6
#define SOLAR_MOUSE_BUTTON_7        ::Solar::Mouse::Button7
#define SOLAR_MOUSE_BUTTON_8        ::Solar::Mouse::Button8
#define SOLAR_MOUSE_BUTTON_LAST     ::Solar::Mouse::ButtonLast
#define SOLAR_MOUSE_BUTTON_LEFT     ::Solar::Mouse::ButtonLeft
#define SOLAR_MOUSE_BUTTON_RIGHT    ::Solar::Mouse::ButtonRight
#define SOLAR_MOUSE_BUTTON_MIDDLE   ::Solar::Mouse::ButtonMiddle
