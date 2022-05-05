#include "solpch.h"
#include "Scene.h"

#include "Components.h"
#include "Solar/Renderer/Renderer2D.h"

namespace Solar {

    static void DoMath(const glm::mat4 tranform) {

    }

    Scene::Scene() {
        //struct TransformComponent {
        //    glm::mat4 Transform;

        //    TransformComponent() = default;
        //    TransformComponent(const TransformComponent&) = default;
        //    TransformComponent(const glm::mat4 transform)
        //      : Transform(transform) {}
        //};

        //TransformComponent transform;
        //DoMath(transform.Transform);


        //entt::entity entity = m_Registry.create();
    }

    Scene::~Scene() {

    }

    entt::entity Scene::CreateEntity() {
        return m_Registry.create();
    }

    void Scene::OnUpdate(TimeStep& ts) {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group) {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform.Transform, sprite.Color);
        }
    }

}
