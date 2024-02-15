// Copyright (c) 2024 Sylar129

#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
namespace solar::math {

bool DecomposeTranform(const glm::mat4& transform, glm::vec3& out_translation,
                       glm::vec3& out_rotation, glm::vec3& out_scale);

}  // namespace solar::math
