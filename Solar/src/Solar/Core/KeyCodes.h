#pragma once

namespace Solar {
    enum class KeyCode : uint16_t {
        // From glfw3.h
        Space                   = 32,
        Apostrophe              = 39,  /* ' */
        Comma                   = 44,  /* , */
        Minus                   = 45,  /* - */
        Period                  = 46,  /* . */
        Slash                   = 47,  /* / */

        D0                      = 48,  /* 0 */
        D1                      = 49,  /* 1 */
        D2                      = 50,  /* 2 */
        D3                      = 51,  /* 3 */
        D4                      = 52,  /* 4 */
        D5                      = 53,  /* 5 */
        D6                      = 54,  /* 6 */
        D7                      = 55,  /* 7 */
        D8                      = 56,  /* 8 */
        D9                      = 57,  /* 9 */

        Semicolon               = 59,  /* ; */
        Equal                   = 61,  /* = */

        A                       = 65,
        B                       = 66,
        C                       = 67,
        D                       = 68,
        E                       = 69,
        F                       = 70,
        G                       = 71,
        H                       = 72,
        I                       = 73,
        J                       = 74,
        K                       = 75,
        L                       = 76,
        M                       = 77,
        N                       = 78,
        O                       = 79,
        P                       = 80,
        Q                       = 81,
        R                       = 82,
        S                       = 83,
        T                       = 84,
        U                       = 85,
        V                       = 86,
        W                       = 87,
        X                       = 88,
        Y                       = 89,
        Z                       = 90,

        LeftBracket             = 91,  /* [ */
        Backslash               = 92,  /* \ */
        RightBracket            = 93,  /* ] */
        GraveAccent             = 96,  /* ` */

        World1                  = 161, /* non-US #1 */
        World2                  = 162, /* non-US #2 */

        /* Function keys */
        Escape                  = 256,
        Enter                   = 257,
        Tab                     = 258,
        Backspace               = 259,
        Insert                  = 260,
        Delete                  = 261,
        Right                   = 262,
        Left                    = 263,
        Down                    = 264,
        Up                      = 265,
        PageUp                  = 266,
        PageDown                = 267,
        Home                    = 268,
        End                     = 269,
        CapsLock                = 280,
        ScrollLock              = 281,
        NumLock                 = 282,
        PrintScreen             = 283,
        Pause                   = 284,
        F1                      = 290,
        F2                      = 291,
        F3                      = 292,
        F4                      = 293,
        F5                      = 294,
        F6                      = 295,
        F7                      = 296,
        F8                      = 297,
        F9                      = 298,
        F10                     = 299,
        F11                     = 300,
        F12                     = 301,
        F13                     = 302,
        F14                     = 303,
        F15                     = 304,
        F16                     = 305,
        F17                     = 306,
        F18                     = 307,
        F19                     = 308,
        F20                     = 309,
        F21                     = 310,
        F22                     = 311,
        F23                     = 312,
        F24                     = 313,
        F25                     = 314,

        /* Keypad */
        KP0                     = 320,
        KP1                     = 321,
        KP2                     = 322,
        KP3                     = 323,
        KP4                     = 324,
        KP5                     = 325,
        KP6                     = 326,
        KP7                     = 327,
        KP8                     = 328,
        KP9                     = 329,
        KPDecimal               = 330,
        KPDivide                = 331,
        KPMultiply              = 332,
        KPSubtract              = 333,
        KPAdd                   = 334,
        KPEnter                 = 335,
        KPEqual                 = 336,

        LeftShift               = 340,
        LeftControl             = 341,
        LeftAlt                 = 342,
        LeftSuper               = 343,
        RightShift              = 344,
        RightControl            = 345,
        RightAlt                = 346,
        RightSuper              = 347,
        Menu                    = 348,
    };

    inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
    {
        os << static_cast<uint16_t>(keyCode);
        return os;
    }
}


// From glfw3.h
#define SOLAR_KEY_SPACE             ::Solar::Key::Space
#define SOLAR_KEY_APOSTROPHE        ::Solar::Key::Apostrophe    /* ' */
#define SOLAR_KEY_COMMA             ::Solar::Key::Comma         /* , */
#define SOLAR_KEY_MINUS             ::Solar::Key::Minus         /* - */
#define SOLAR_KEY_PERIOD            ::Solar::Key::Period        /* . */
#define SOLAR_KEY_SLASH             ::Solar::Key::Slash         /* / */
#define SOLAR_KEY_0                 ::Solar::Key::D0
#define SOLAR_KEY_1                 ::Solar::Key::D1
#define SOLAR_KEY_2                 ::Solar::Key::D2
#define SOLAR_KEY_3                 ::Solar::Key::D3
#define SOLAR_KEY_4                 ::Solar::Key::D4
#define SOLAR_KEY_5                 ::Solar::Key::D5
#define SOLAR_KEY_6                 ::Solar::Key::D6
#define SOLAR_KEY_7                 ::Solar::Key::D7
#define SOLAR_KEY_8                 ::Solar::Key::D8
#define SOLAR_KEY_9                 ::Solar::Key::D9
#define SOLAR_KEY_SEMICOLON         ::Solar::Key::Semicolon     /* ; */
#define SOLAR_KEY_EQUAL             ::Solar::Key::Equal         /* = */
#define SOLAR_KEY_A                 ::Solar::Key::A
#define SOLAR_KEY_B                 ::Solar::Key::B
#define SOLAR_KEY_C                 ::Solar::Key::C
#define SOLAR_KEY_D                 ::Solar::Key::D
#define SOLAR_KEY_E                 ::Solar::Key::E
#define SOLAR_KEY_F                 ::Solar::Key::F
#define SOLAR_KEY_G                 ::Solar::Key::G
#define SOLAR_KEY_H                 ::Solar::Key::H
#define SOLAR_KEY_I                 ::Solar::Key::I
#define SOLAR_KEY_J                 ::Solar::Key::J
#define SOLAR_KEY_K                 ::Solar::Key::K
#define SOLAR_KEY_L                 ::Solar::Key::L
#define SOLAR_KEY_M                 ::Solar::Key::M
#define SOLAR_KEY_N                 ::Solar::Key::N
#define SOLAR_KEY_O                 ::Solar::Key::O
#define SOLAR_KEY_P                 ::Solar::Key::P
#define SOLAR_KEY_Q                 ::Solar::Key::Q
#define SOLAR_KEY_R                 ::Solar::Key::R
#define SOLAR_KEY_S                 ::Solar::Key::S
#define SOLAR_KEY_T                 ::Solar::Key::T
#define SOLAR_KEY_U                 ::Solar::Key::U
#define SOLAR_KEY_V                 ::Solar::Key::V
#define SOLAR_KEY_W                 ::Solar::Key::W
#define SOLAR_KEY_X                 ::Solar::Key::X
#define SOLAR_KEY_Y                 ::Solar::Key::Y
#define SOLAR_KEY_Z                 ::Solar::Key::Z
#define SOLAR_KEY_LEFT_BRACKET      ::Solar::Key::LeftBracket   /* [ */
#define SOLAR_KEY_BACKSLASH         ::Solar::Key::Backslash     /* \ */
#define SOLAR_KEY_RIGHT_BRACKET     ::Solar::Key::RightBracket  /* ] */
#define SOLAR_KEY_GRAVE_ACCENT      ::Solar::Key::GraveAccent   /* ` */
#define SOLAR_KEY_WORLD_1           ::Solar::Key::World1        /* non-US #1 */
#define SOLAR_KEY_WORLD_2           ::Solar::Key::World2        /* non-US #2 */

/* Function keys */
#define SOLAR_KEY_ESCAPE            ::Solar::Key::Escape
#define SOLAR_KEY_ENTER             ::Solar::Key::Enter
#define SOLAR_KEY_TAB               ::Solar::Key::Tab
#define SOLAR_KEY_BACKSPACE         ::Solar::Key::Backspace
#define SOLAR_KEY_INSERT            ::Solar::Key::Insert
#define SOLAR_KEY_DELETE            ::Solar::Key::Delete
#define SOLAR_KEY_RIGHT             ::Solar::Key::Right
#define SOLAR_KEY_LEFT              ::Solar::Key::Left
#define SOLAR_KEY_DOWN              ::Solar::Key::Down
#define SOLAR_KEY_UP                ::Solar::Key::Up
#define SOLAR_KEY_PAGE_UP           ::Solar::Key::PageUp
#define SOLAR_KEY_PAGE_DOWN         ::Solar::Key::PageDown
#define SOLAR_KEY_HOME              ::Solar::Key::Home
#define SOLAR_KEY_END               ::Solar::Key::End
#define SOLAR_KEY_CAPS_LOCK         ::Solar::Key::CapsLock
#define SOLAR_KEY_SCROLL_LOCK       ::Solar::Key::ScrollLock
#define SOLAR_KEY_NUM_LOCK          ::Solar::Key::NumLock
#define SOLAR_KEY_PRINT_SCREEN      ::Solar::Key::PrintScreen
#define SOLAR_KEY_PAUSE             ::Solar::Key::Pause
#define SOLAR_KEY_F1                ::Solar::Key::F1
#define SOLAR_KEY_F2                ::Solar::Key::F2
#define SOLAR_KEY_F3                ::Solar::Key::F3
#define SOLAR_KEY_F4                ::Solar::Key::F4
#define SOLAR_KEY_F5                ::Solar::Key::F5
#define SOLAR_KEY_F6                ::Solar::Key::F6
#define SOLAR_KEY_F7                ::Solar::Key::F7
#define SOLAR_KEY_F8                ::Solar::Key::F8
#define SOLAR_KEY_F9                ::Solar::Key::F9
#define SOLAR_KEY_F10               ::Solar::Key::F10
#define SOLAR_KEY_F11               ::Solar::Key::F11
#define SOLAR_KEY_F12               ::Solar::Key::F12
#define SOLAR_KEY_F13               ::Solar::Key::F13
#define SOLAR_KEY_F14               ::Solar::Key::F14
#define SOLAR_KEY_F15               ::Solar::Key::F15
#define SOLAR_KEY_F16               ::Solar::Key::F16
#define SOLAR_KEY_F17               ::Solar::Key::F17
#define SOLAR_KEY_F18               ::Solar::Key::F18
#define SOLAR_KEY_F19               ::Solar::Key::F19
#define SOLAR_KEY_F20               ::Solar::Key::F20
#define SOLAR_KEY_F21               ::Solar::Key::F21
#define SOLAR_KEY_F22               ::Solar::Key::F22
#define SOLAR_KEY_F23               ::Solar::Key::F23
#define SOLAR_KEY_F24               ::Solar::Key::F24
#define SOLAR_KEY_F25               ::Solar::Key::F25

/* Keypad */
#define SOLAR_KEY_KP_0              ::Solar::Key::KP0
#define SOLAR_KEY_KP_1              ::Solar::Key::KP1
#define SOLAR_KEY_KP_2              ::Solar::Key::KP2
#define SOLAR_KEY_KP_3              ::Solar::Key::KP3
#define SOLAR_KEY_KP_4              ::Solar::Key::KP4
#define SOLAR_KEY_KP_5              ::Solar::Key::KP5
#define SOLAR_KEY_KP_6              ::Solar::Key::KP6
#define SOLAR_KEY_KP_7              ::Solar::Key::KP7
#define SOLAR_KEY_KP_8              ::Solar::Key::KP8
#define SOLAR_KEY_KP_9              ::Solar::Key::KP9
#define SOLAR_KEY_KP_DECIMAL        ::Solar::Key::KPDecimal
#define SOLAR_KEY_KP_DIVIDE         ::Solar::Key::KPDivide
#define SOLAR_KEY_KP_MULTIPLY       ::Solar::Key::KPMultiply
#define SOLAR_KEY_KP_SUBTRACT       ::Solar::Key::KPSubtract
#define SOLAR_KEY_KP_ADD            ::Solar::Key::KPAdd
#define SOLAR_KEY_KP_ENTER          ::Solar::Key::KPEnter
#define SOLAR_KEY_KP_EQUAL          ::Solar::Key::KPEqual

#define SOLAR_KEY_LEFT_SHIFT        ::Solar::Key::LeftShift
#define SOLAR_KEY_LEFT_CONTROL      ::Solar::Key::LeftControl
#define SOLAR_KEY_LEFT_ALT          ::Solar::Key::LeftAlt
#define SOLAR_KEY_LEFT_SUPER        ::Solar::Key::LeftSuper
#define SOLAR_KEY_RIGHT_SHIFT       ::Solar::Key::RightShift
#define SOLAR_KEY_RIGHT_CONTROL     ::Solar::Key::RightControl
#define SOLAR_KEY_RIGHT_ALT         ::Solar::Key::RightAlt
#define SOLAR_KEY_RIGHT_SUPER       ::Solar::Key::RightSuper
#define SOLAR_KEY_MENU              ::Solar::Key::Menu

#define SOLAR_KEY_LAST               SOLAR_KEY_MENU
