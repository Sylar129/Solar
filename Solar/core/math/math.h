// Copyright (c) 2024 Sylar129

#pragma once

#include "glm/glm.hpp"

namespace solar::Math {

bool DecomposeTranform(const glm::mat4& transform, glm::vec3& out_translation,
                       glm::vec3& out_rotation, glm::vec3& out_scale);

}  // namespace solar::Math