// Copyright (c) 2024 Sylar129

#include "core/scene/scene.h"

#include "core/renderer/renderer2d.h"
#include "core/scene/components.h"
#include "core/scene/entity.h"

namespace solar {

Scene::Scene() {}

Scene::~Scene() {}

Entity Scene::CreateEntity(const std::string& name) {
  Entity entity = {registry_.create(), this};
  entity.AddComponent<TransformComponent>();
  auto& tag = entity.AddComponent<TagComponent>();
  tag.tag = name.empty() ? "Entity" : name;
  return entity;
}

void Scene::DestoryEntity(Entity entity) { registry_.destroy(entity); }

void Scene::OnUpdateRuntime(const TimeStep& ts) {
  // Update scripts
  {
    // TODO(sylar): Move to Scene::OnScenePlay
    registry_.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
      if (!nsc.instance) {
        nsc.instance = nsc.instantiate_script();
        nsc.instance->entity_ = Entity{entity, this};
        nsc.instance->OnCreate();
      }
      nsc.instance->OnUpdate(ts);
    });
  }

  // Render 2D
  Camera* main_camera = nullptr;
  glm::mat4 camera_transform;
  {
    auto view = registry_.view<CameraComponent, TransformComponent>();
    for (const auto& entity : view) {
      auto [transform, camera] =
          view.get<TransformComponent, CameraComponent>(entity);

      if (camera.primary) {
        main_camera = &camera.camera;
        camera_transform = transform.GetTranform();
        break;
      }
    }
  }

  if (main_camera) {
    Renderer2D::BeginScene(*main_camera, camera_transform);
    auto group =
        registry_.group<TransformComponent>(entt::get<SpriteRendererComponent>);
    for (const auto& entity : group) {
      auto [transform, sprite] =
          group.get<TransformComponent, SpriteRendererComponent>(entity);

      // Renderer2D::DrawQuad(transform.GetTranform(), sprite.Color);
      Renderer2D::DrawSprite(transform, sprite, static_cast<int>(entity));
    }
    Renderer2D::EndScene();
  }
}

void Scene::OnUpdateEditor(const TimeStep& ts, EditorCamera& camera) {
  Renderer2D::BeginScene(camera);
  auto group =
      registry_.group<TransformComponent>(entt::get<SpriteRendererComponent>);
  for (const auto& entity : group) {
    auto [transform, sprite] =
        group.get<TransformComponent, SpriteRendererComponent>(entity);

    // Renderer2D::DrawQuad(transform.GetTranform(), sprite.Color);
    Renderer2D::DrawSprite(transform, sprite, static_cast<int>(entity));
  }
  Renderer2D::EndScene();
}

void Scene::OnViewportResize(const Size& size) {
  viewport_size_ = size;

  // Resize our non-FixedAspectRatio cameras
  auto view = registry_.view<CameraComponent>();
  for (const auto& entity : view) {
    auto& camera_component = view.get<CameraComponent>(entity);
    if (!camera_component.fixed_aspect_ratio) {
      camera_component.camera.SetViewportSize(size);
    }
  }
}

Entity Scene::GetPrimaryCameraEntity() {
  auto view = registry_.view<CameraComponent>();
  for (const auto& entity : view) {
    const auto& camera = view.get<CameraComponent>(entity);
    if (camera.primary) {
      return Entity{entity, this};
    }
  }
  return Entity();
}

template <typename T>
void Scene::OnComponentAdded(Entity entity, T& component) {
  // static_assert(false);
}

template <>
void Scene::OnComponentAdded<TransformComponent>(
    Entity entity, TransformComponent& component) {}

template <>
void Scene::OnComponentAdded<CameraComponent>(Entity entity,
                                              CameraComponent& component) {
  component.camera.SetViewportSize(viewport_size_);
}

template <>
void Scene::OnComponentAdded<SpriteRendererComponent>(
    Entity entity, SpriteRendererComponent& component) {}

template <>
void Scene::OnComponentAdded<TagComponent>(Entity entity,
                                           TagComponent& component) {}

template <>
void Scene::OnComponentAdded<NativeScriptComponent>(
    Entity entity, NativeScriptComponent& component) {}

}  // namespace solar
