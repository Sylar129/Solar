// Copyright (c) 2024 Sylar129

#pragma once

#include <utility>

#include "Core/Base/Sundry.h"
#include "Core/Base/Log.h"
#include "Core/Scene/Scene.h"
#include "entt/entt.hpp"

namespace solar {

class Entity {
 public:
  Entity() = default;
  Entity(entt::entity handle, Scene* scene);
  Entity(const Entity& other) = default;

  template <typename T, typename... Args>
  T& AddComponent(Args&&... args) {
    SOLAR_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
    T& component = scene_->registry_.emplace<T>(entity_handle_,
                                                std::forward<Args>(args)...);

    scene_->OnComponentAdded<T>(*this, component);

    return component;
  }

  template <typename T>
  T& GetComponent() {
    SOLAR_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

    return scene_->registry_.get<T>(entity_handle_);
  }

  template <typename T>
  bool HasComponent() {
    return scene_->registry_.any_of<T>(entity_handle_);
  }

  template <typename T>
  void RemoveComponent() {
    SOLAR_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

    scene_->registry_.remove<T>(entity_handle_);
  }

  operator bool() const { return entity_handle_ != entt::null; }
  operator uint32_t() const { return (uint32_t)entity_handle_; }
  operator entt::entity() const { return entity_handle_; }

  bool operator==(const Entity& other) const {
    return entity_handle_ == other.entity_handle_ && scene_ == other.scene_;
  }
  bool operator!=(const Entity& other) const { return !operator==(other); }

 private:
  entt::entity entity_handle_{entt::null};
  Scene* scene_ = nullptr;
};

}  // namespace solar
