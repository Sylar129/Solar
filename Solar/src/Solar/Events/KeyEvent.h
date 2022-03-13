#pragma once

#include "Event.h"

namespace Solar {
    class SOLAR_API KeyEvent : public Event {
    public:
        inline unsigned int GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(int keycode) : m_KeyCode(keycode) {}

        unsigned int m_KeyCode;
    };

    class SOLAR_API KeyPressdEvent :public KeyEvent {
    public:
        KeyPressdEvent(unsigned int keycode, int repeatCount)
          : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

        inline int GetRepeatCount() const { return m_RepeatCount; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

    class SOLAR_API KeyReleasedEvent :public KeyEvent {
    public:
        KeyReleasedEvent(unsigned int keycode) : KeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class SOLAR_API KeyTypedEvent :public KeyEvent {
    public:
        KeyTypedEvent(unsigned int keycode) : KeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}
