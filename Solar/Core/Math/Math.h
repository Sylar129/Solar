#pragma once

#include "glm/glm.hpp"

namespace Solar::Math {

bool DecomposeTranform(const glm::mat4& transform, glm::vec3& outTranslation,
                       glm::vec3& outRotation, glm::vec3& outScale);

}  // namespace Solar::Math
