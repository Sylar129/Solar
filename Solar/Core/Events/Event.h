// Copyright (c) 2024 Sylar129

#pragma once

#include <functional>
#include <ostream>
#include <string>

#include "Core/Base/Sundry.h"

namespace solar {

enum class EventType {
  kNone = 0,
  kWindowClose,
  kWindowResize,
  kWindowFocus,
  kWindowLostFocus,
  kWindowMoved,
  kApptick,
  kAppUpdate,
  kAppRender,
  kKeyPressed,
  kKeyReleased,
  kKeyTyped,
  kMouseButtonPressed,
  kMouseButtonReleased,
  kMouseMoved,
  kMouseScrolled
};

enum EventCategory {
  kNone = 0,
  kEventCategoryApplication = BIT(0),
  kEventCategoryInput = BIT(1),
  kEventCategoryKeyboard = BIT(2),
  kEventCategoryMouse = BIT(3),
  kEventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                        \
  static EventType GetStaticType() { return EventType::type; }        \
  EventType GetEventType() const override { return GetStaticType(); } \
  const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) \
  int GetCategoryFlags() const override { return category; }

class Event {
  friend class EventDispatcher;

 public:
  virtual ~Event() = default;

  bool handled = false;

  virtual EventType GetEventType() const = 0;
  virtual const char* GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual std::string ToString() const { return GetName(); }

  inline bool IsInCategory(EventCategory category) const {
    return GetCategoryFlags() & category;
  }
};

class EventDispatcher {
  template <typename T>
  using EventFn = std::function<bool(T&)>;

 public:
  explicit EventDispatcher(Event& event) : event_(event) {}

  template <typename T>
  bool Dispatch(EventFn<T> func) {
    if (event_.GetEventType() == T::GetStaticType()) {
      event_.handled = func(*static_cast<T*>(&event_));
      return true;
    }
    return false;
  }

 private:
  Event& event_;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}

}  // namespace solar
