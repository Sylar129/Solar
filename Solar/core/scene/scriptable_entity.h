// Copyright (c) 2024 Sylar129

#pragma once

#include "core/scene/entity.h"

namespace solar {

class ScriptableEntity {
 public:
  virtual ~ScriptableEntity() {}

  template <typename T>
  T& GetComponent() {
    return entity_.GetComponent<T>();
  }

 protected:
  virtual void OnCreate() {}
  virtual void OnDestroy() {}
  virtual void OnUpdate(const TimeStep& ts) {}

 private:
  Entity entity_;
  friend class Scene;
};

}  // namespace solar
