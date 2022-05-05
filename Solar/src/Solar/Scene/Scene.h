#pragma once

#include "entt.hpp"
#include "Solar/Core/Timestep.h"

namespace Solar {

    class Scene {
    public:
        Scene();
        ~Scene();

        entt::entity CreateEntity();

        // TEMP
        entt::registry& Reg() { return m_Registry; }

        void OnUpdate(TimeStep& ts);
    private:
        entt::registry m_Registry;

    };
}
