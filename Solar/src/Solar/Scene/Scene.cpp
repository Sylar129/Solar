#include "solpch.h"
#include "Scene.h"

#include "Components.h"
#include "Solar/Renderer/Renderer2D.h"

#include "Entity.h"

namespace Solar {

    static void DoMath(const glm::mat4 tranform) {

    }

    Scene::Scene() {

    }

    Scene::~Scene() {

    }

    Entity Scene::CreateEntity(const std::string& name) {
        Entity entity = { m_Registry.create(),this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::OnUpdate(TimeStep& ts) {
        // Render 2D
        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;
        {
            auto group = m_Registry.view<CameraComponent, TransformComponent>();
            for (auto entity : group) {
                auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

                if (camera.Primary) {
                    mainCamera = &camera.Camera;
                    cameraTransform = &transform.Transform;
                    break;
                }
            }
        }

        if (mainCamera) {
            Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group) {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform.Transform, sprite.Color);
            }
            Renderer2D::EndScene();
        }
    }
}
