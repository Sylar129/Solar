// Copyright (c) 2024 Sylar129

#pragma once

#include <sstream>
#include <string>

#include "Core/Base/MouseCodes.h"
#include "Core/Events/Event.h"

namespace Solar {

class MouseMovedEvent : public Event {
 public:
  MouseMovedEvent(float x, float y) : mouse_x_(x), mouse_y_(y) {}

  inline float GetX() const { return mouse_x_; }
  inline float GetY() const { return mouse_y_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent: " << mouse_x_ << ", " << mouse_y_;
    return ss.str();
  }
  EVENT_CLASS_TYPE(kMouseMoved);
  EVENT_CLASS_CATEGORY(kEventCategoryMouse | kEventCategoryInput);

 private:
  float mouse_x_;
  float mouse_y_;
};

class MouseScrolledEvent : public Event {
 public:
  MouseScrolledEvent(float x_offset, float y_offset)
      : x_offset_(x_offset), y_offset_(y_offset) {}

  inline float GetXOffset() const { return x_offset_; }
  inline float GetYOffset() const { return y_offset_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseScrolledEvent: " << x_offset_ << ", " << y_offset_;
    return ss.str();
  }
  EVENT_CLASS_TYPE(kMouseScrolled);
  EVENT_CLASS_CATEGORY(kEventCategoryMouse | kEventCategoryInput);

 private:
  float x_offset_;
  float y_offset_;
};

class MouseButtonEvent : public Event {
 public:
  inline MouseCode GetMouseButton() const { return button_; }

  EVENT_CLASS_CATEGORY(kEventCategoryMouse | kEventCategoryInput |
                       kEventCategoryMouseButton);

 protected:
  explicit MouseButtonEvent(MouseCode button) : button_(button) {}

  MouseCode button_;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
 public:
  explicit MouseButtonPressedEvent(MouseCode button)
      : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << button_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(kMouseButtonPressed);
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
 public:
  explicit MouseButtonReleasedEvent(MouseCode button)
      : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << button_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(kMouseButtonReleased);
};

}  // namespace Solar
