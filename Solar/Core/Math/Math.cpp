// Copyright (c) 2024 Sylar129

#include "Core/Math/Math.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace Solar::Math {

bool DecomposeTranform(const glm::mat4& transform, glm::vec3& out_translation,
                       glm::vec3& out_rotation, glm::vec3& out_scale) {
  // From glm::decompose in matrix_decompose.inl

  using namespace glm;
  using T = float;

  mat4 local_matrix(transform);

  // Normalize the matrix.
  if (epsilonEqual(local_matrix[3][3], static_cast<T>(0), epsilon<T>()))
    return false;

  // First, isolate perspective.  This is the messiest.
  if (epsilonNotEqual(local_matrix[0][3], static_cast<T>(0), epsilon<T>()) ||
      epsilonNotEqual(local_matrix[1][3], static_cast<T>(0), epsilon<T>()) ||
      epsilonNotEqual(local_matrix[2][3], static_cast<T>(0), epsilon<T>())) {
    // Clear the perspective partition
    local_matrix[0][3] = local_matrix[1][3] = local_matrix[2][3] =
        static_cast<T>(0);
    local_matrix[3][3] = static_cast<T>(1);
  }

  // Next take care of translation (easy).
  out_translation = vec3(local_matrix[3]);
  local_matrix[3] = vec4(0, 0, 0, local_matrix[3].w);

  vec3 Row[3]{};

  // Now get scale and shear.
  for (length_t i = 0; i < 3; ++i)
    for (length_t j = 0; j < 3; ++j) Row[i][j] = local_matrix[i][j];

  // Compute X scale factor and normalize first row.
  out_scale.x = length(Row[0]);
  Row[0] = detail::scale(Row[0], static_cast<T>(1));
  out_scale.y = length(Row[1]);
  Row[1] = detail::scale(Row[1], static_cast<T>(1));
  out_scale.z = length(Row[2]);
  Row[2] = detail::scale(Row[2], static_cast<T>(1));

  // At this point, the matrix (in rows[]) is orthonormal.
  // Check for a coordinate system flip.  If the determinant
  // is -1, then negate the matrix and the scaling factors.
#if 0
    vec3 Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
    if (dot(Row[0], Pdum3) < 0) {
        for (length_t i = 0; i < 3; i++) {
            scale[i] *= static_cast<T>(-1);
            Row[i] *= static_cast<T>(-1);
        }
    }
#endif

  out_rotation.y = asin(-Row[0][2]);
  if (cos(out_rotation.y) != 0) {
    out_rotation.x = atan2(Row[1][2], Row[2][2]);
    out_rotation.z = atan2(Row[0][1], Row[0][0]);
  } else {
    out_rotation.x = atan2(-Row[2][0], Row[1][1]);
    out_rotation.z = 0;
  }

  return true;
}

}  // namespace Solar::Math
