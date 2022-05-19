#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Solar {

    struct TagComponent {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {
        }
    };

    struct TransformComponent {
        glm::mat4 Transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
            : Transform(transform) {
        }
    };

    struct SpriteRendererComponent {
        glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            : Color(color) {
        }
    };

    struct CameraComponent {
        SceneCamera Camera;
        bool Primary = true; // TODO: Move to the scene
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    struct NativeScriptComponent {
        ScriptableEntity* Instance = nullptr;

        std::function<void()> InstantiateFunction;
        std::function<void()> DestoryInstanceFunction;

        std::function<void(ScriptableEntity*)> OnCreateFunction;
        std::function<void(ScriptableEntity*)> OnDestoryFunction;
        std::function<void(ScriptableEntity* , TimeStep)> OnUpdateFunction;

        template<typename T>
        void Bind() {
            InstantiateFunction = [&]() { Instance = new T(); };
            DestoryInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

            OnCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
            OnDestoryFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnDestory(); };
            OnUpdateFunction = [](ScriptableEntity* instance, TimeStep ts) { ((T*)instance)->OnUpdate(ts); };
        }
    };
}
