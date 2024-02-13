// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Scene/Entity.h"

namespace Solar {

class ScriptableEntity {
 public:
  virtual ~ScriptableEntity(){};

  template <typename T>
  T& GetComponent() {
    return entity_.GetComponent<T>();
  }

 protected:
  virtual void OnCreate() {}
  virtual void OnDestroy() {}
  virtual void OnUpdate(TimeStep ts) {}

 private:
  Entity entity_;
  friend class Scene;
};

}  // namespace Solar
