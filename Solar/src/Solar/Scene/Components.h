#pragma once

#include <glm/glm.hpp>

#include "Solar/Renderer/Camera.h"

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
        Solar::Camera Camera;
        bool Primary = true; // TODO: Move to the scene

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const glm::mat4& projection)
            : Camera(projection) {
        }
    };
}
