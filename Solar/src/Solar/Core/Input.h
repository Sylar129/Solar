#pragma once

#include "Solar/Core/KeyCodes.h"
#include "Solar/Core/MouseCodes.h"

namespace Solar {

class Input {
public:
    static bool IsKeyPressed(KeyCode keycode);
    static bool IsMouseButtonPressed(MouseCode button);
    static std::pair<float, float> GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};

} // namespace Solar
