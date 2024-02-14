// Copyright (c) 2024 Sylar129

#pragma once

#include <sstream>
#include <string>

#include "core/events/event.h"

namespace solar {

class WindowResizeEvent : public Event {
 public:
  WindowResizeEvent(unsigned int width, unsigned int height)
      : width_(width), height_(height) {}

  inline unsigned int GetWidth() const { return width_; }
  inline unsigned int GetHeight() const { return height_; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << width_ << ", " << height_;
    return ss.str();
  }
  EVENT_CLASS_TYPE(kWindowResize);
  EVENT_CLASS_CATEGORY(kEventCategoryApplication);

 private:
  unsigned int width_;
  unsigned int height_;
};

class WindowCloseEvent : public Event {
 public:
  WindowCloseEvent() {}

  EVENT_CLASS_TYPE(kWindowClose);
  EVENT_CLASS_CATEGORY(kEventCategoryApplication);
};

class AppTickEvent : public Event {
 public:
  AppTickEvent() {}

  EVENT_CLASS_TYPE(kApptick);
  EVENT_CLASS_CATEGORY(kEventCategoryApplication);
};

class AppUpdateEvent : public Event {
 public:
  AppUpdateEvent() {}

  EVENT_CLASS_TYPE(kAppUpdate);
  EVENT_CLASS_CATEGORY(kEventCategoryApplication);
};

class AppRenderEvent : public Event {
 public:
  AppRenderEvent() {}

  EVENT_CLASS_TYPE(kAppRender);
  EVENT_CLASS_CATEGORY(kEventCategoryApplication);
};

}  // namespace solar
