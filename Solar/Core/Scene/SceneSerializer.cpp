// Copyright (c) 2024 Sylar129

#include "Core/Scene/SceneSerializer.h"

#include <ostream>
#include <string>

#include "Core/Base/Log.h"
#include "Core/Scene/Components.h"
#include "Core/Scene/Entity.h"
#include "yaml-cpp/yaml.h"

namespace YAML {

template <>
struct convert<glm::vec3> {
  static Node encode(const glm::vec3& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    return node;
  }

  static bool decode(const Node& node, glm::vec3& rhs) {
    if (!node.IsSequence() || node.size() != 3) {
      return false;
    }

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    return true;
  }
};

template <>
struct convert<glm::vec4> {
  static Node encode(const glm::vec4& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    node.push_back(rhs.w);
    return node;
  }

  static bool decode(const Node& node, glm::vec4& rhs) {
    if (!node.IsSequence() || node.size() != 4) {
      return false;
    }

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    rhs.w = node[3].as<float>();
    return true;
  }
};

}  // namespace YAML

namespace Solar {

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
  return out;
}

SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : scene_(scene) {}

static void SerializeEntity(YAML::Emitter& out, Entity entity) {
  out << YAML::BeginMap;  // Entity
  out << YAML::Key << "Entity" << YAML::Value << "710288202";

  // TagComponent
  if (entity.HasComponent<TagComponent>()) {
    out << YAML::Key << "TagComponent";
    out << YAML::BeginMap;

    auto& tag = entity.GetComponent<TagComponent>().tag;
    out << YAML::Key << "Tag" << YAML::Value << tag;

    out << YAML::EndMap;
  }

  // TransformComponent
  if (entity.HasComponent<TransformComponent>()) {
    out << YAML::Key << "TransformComponent";
    out << YAML::BeginMap;

    auto& transform_component = entity.GetComponent<TransformComponent>();
    out << YAML::Key << "Translation" << YAML::Value
        << transform_component.translation;
    out << YAML::Key << "Rotation" << YAML::Value
        << transform_component.rotation;
    out << YAML::Key << "Scale" << YAML::Value << transform_component.scale;

    out << YAML::EndMap;
  }

  // CameraComponent
  if (entity.HasComponent<CameraComponent>()) {
    out << YAML::Key << "CameraComponent";
    out << YAML::BeginMap;

    auto& camera_component = entity.GetComponent<CameraComponent>();
    auto& camera = camera_component.camera;

    out << YAML::Key << "Camera" << YAML::Value;
    out << YAML::BeginMap;
    out << YAML::Key << "ProjectionType" << YAML::Value
        << static_cast<int>(camera.GetProjectionType());
    out << YAML::Key << "PerspectiveFOV" << YAML::Value
        << camera.GetPerspectiveVerticalFOV();
    out << YAML::Key << "PerspectiveNear" << YAML::Value
        << camera.GetPerspectiveNearClip();
    out << YAML::Key << "PerspectiveFar" << YAML::Value
        << camera.GetPerspectiveFarClip();
    out << YAML::Key << "OrthographicSize" << YAML::Value
        << camera.GetOrthographicSize();
    out << YAML::Key << "OrthographicNear" << YAML::Value
        << camera.GetOrthographicNearClip();
    out << YAML::Key << "OrthographicFar" << YAML::Value
        << camera.GetOrthographicFarClip();

    out << YAML::EndMap;

    out << YAML::Key << "Primary" << YAML::Value << camera_component.primary;
    out << YAML::Key << "FixedAspectRatio" << YAML::Value
        << camera_component.fixed_aspect_ratio;

    out << YAML::EndMap;
  }

  // SpriteRendererComponent
  if (entity.HasComponent<SpriteRendererComponent>()) {
    out << YAML::Key << "SpriteRendererComponent";
    out << YAML::BeginMap;

    auto& sprite_renderer_component =
        entity.GetComponent<SpriteRendererComponent>();
    out << YAML::Key << "Color" << YAML::Value
        << sprite_renderer_component.color;

    out << YAML::EndMap;
  }

  out << YAML::EndMap;  // Entity
}

void SceneSerializer::Serialize(const std::string& filepath) {
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "Scene" << YAML::Value << "TODO: SceneName";
  out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
  for (auto [entityID, _] : scene_->registry_.storage<Entity>().each()) {
    Entity entity = {entityID, scene_.get()};
    if (!entity) {
      return;
    }
    SerializeEntity(out, entity);
  }
  out << YAML::EndSeq;
  out << YAML::EndMap;

  std::ofstream fout(filepath);
  fout << out.c_str();
}

bool SceneSerializer::Deserialize(const std::string& filepath) {
  std::ifstream stream(filepath);
  std::stringstream str_stream;
  str_stream << stream.rdbuf();

  YAML::Node data = YAML::Load(str_stream.str());
  if (!data["Scene"]) {
    return false;
  }

  std::string scene_name = data["Scene"].as<std::string>();
  SOLAR_CORE_TRACE("Deserializing Scene '{0}'", scene_name);

  auto entities = data["Entities"];
  if (entities) {
    for (const auto& entity : entities) {
      // TODO(sylar): gen uuid
      uint64_t uuid = entity["Entity"].as<uint64_t>();

      std::string name;
      auto tag_component = entity["TagComponent"];
      if (tag_component) {
        name = tag_component["Tag"].as<std::string>();
      }

      SOLAR_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid,
                       name);

      Entity deserialized_entity = scene_->CreateEntity(name);

      // TransformComponent
      auto transform_component = entity["TransformComponent"];
      if (transform_component) {
        // Entities always have transforms
        auto& tc = deserialized_entity.GetComponent<TransformComponent>();
        tc.translation = transform_component["Translation"].as<glm::vec3>();
        tc.rotation = transform_component["Rotation"].as<glm::vec3>();
        tc.scale = transform_component["Scale"].as<glm::vec3>();
      }

      // CameraComponent
      auto camera_component = entity["CameraComponent"];
      if (camera_component) {
        auto& cc = deserialized_entity.AddComponent<CameraComponent>();

        auto camera_props = camera_component["Camera"];
        cc.camera.SetProjectionType(
            (SceneCamera::ProjectionType)camera_props["ProjectionType"]
                .as<int>());

        cc.camera.SetPerspectiveVerticalFOV(
            camera_props["PerspectiveFOV"].as<float>());
        cc.camera.SetPerspectiveNearClip(
            camera_props["PerspectiveNear"].as<float>());
        cc.camera.SetPerspectiveFarClip(
            camera_props["PerspectiveFar"].as<float>());

        cc.camera.SetOrthographicSize(
            camera_props["OrthographicSize"].as<float>());
        cc.camera.SetOrthographicNearClip(
            camera_props["OrthographicNear"].as<float>());
        cc.camera.SetOrthographicFarClip(
            camera_props["OrthographicFar"].as<float>());

        cc.primary = camera_component["Primary"].as<bool>();
        cc.fixed_aspect_ratio = camera_component["FixedAspectRatio"].as<bool>();
      }

      // SpriteRendererComponent
      auto sprite_renderer_component = entity["SpriteRendererComponent"];
      if (sprite_renderer_component) {
        auto& src = deserialized_entity.AddComponent<SpriteRendererComponent>();
        src.color = sprite_renderer_component["Color"].as<glm::vec4>();
      }
    }
  }

  return true;
}

void SceneSerializer::SerializeRuntime(const std::string& filepath) {
  SOLAR_CORE_ASSERT("Not Implemented!", false);
}

bool SceneSerializer::DeserializeRuntime(const std::string& filepath) {
  SOLAR_CORE_ASSERT("Not Implemented!", false);
  return false;
}

}  // namespace Solar
