#include "solpch.h"
#include "Entity.h"

namespace Solar {
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene) {

    }
}
