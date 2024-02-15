// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "core/scene/scene.h"
#include "core/utils/misc.h"

namespace solar {

class SceneSerializer {
 public:
  explicit SceneSerializer(const Ref<Scene>& scene);

  void Serialize(const std::string& filepath);
  bool Deserialize(const std::string& filepath);

  void SerializeRuntime(const std::string& filepath);
  bool DeserializeRuntime(const std::string& filepath);

 private:
  Ref<Scene> scene_;
};

}  // namespace solar
