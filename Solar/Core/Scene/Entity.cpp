// Copyright (c) 2024 Sylar129

#include "Core/Scene/Entity.h"

namespace Solar {

Entity::Entity(entt::entity handle, Scene* scene)
    : entity_handle_(handle), scene_(scene) {}

}  // namespace Solar
