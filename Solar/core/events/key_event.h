// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "core/base/key_codes.h"
#include "core/events/event.h"

namespace solar {

class KeyEvent : public Event {
 public:
  inline KeyCode GetKeyCode() const { return keycode_; }

  EVENT_CLASS_CATEGORY(kEventCategoryKeyboard | kEventCategoryInput);

 protected:
  explicit KeyEvent(KeyCode keycode) : keycode_(keycode) {}

  KeyCode keycode_;
};

class KeyPressdEvent : public KeyEvent {
 public:
  KeyPressdEvent(KeyCode keycode, int repeat_count)
      : KeyEvent(keycode), repeat_count_(repeat_count) {}

  inline int GetRepeatCount() const { return repeat_count_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << keycode_ << " (" << repeat_count_
       << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_TYPE(kKeyPressed);

 private:
  int repeat_count_;
};

class KeyReleasedEvent : public KeyEvent {
 public:
  explicit KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << keycode_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(kKeyReleased);
};

class KeyTypedEvent : public KeyEvent {
 public:
  explicit KeyTypedEvent(KeyCode keycode) : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyTypedEvent: " << keycode_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(kKeyTyped);
};

}  // namespace solar
