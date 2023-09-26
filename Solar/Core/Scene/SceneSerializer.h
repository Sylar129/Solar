#pragma once

#include "Core/Scene/Scene.h"

namespace Solar {

class SceneSerializer {
 public:
  SceneSerializer(const Ref<Scene>& scene);

  void Serialize(const std::string& filepath);
  bool Deserialize(const std::string& filepath);

  void SerializeRuntime(const std::string& filepath);
  bool DeserializeRuntime(const std::string& filepath);

 private:
  Ref<Scene> m_Scene;
};

}  // namespace Solar
