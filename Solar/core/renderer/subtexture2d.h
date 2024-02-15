// Copyright (c) 2024 Sylar129

#pragma once

#include "core/renderer/texture.h"
#include "core/utils/misc.h"
#include "glm/ext/vector_float2.hpp"

namespace solar {

class SubTexture2D {
 public:
  SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min,
               const glm::vec2& max);

  Ref<Texture2D> GetTexture() const { return texture_; }
  const auto& GetTexCoords() const { return tex_coords_; }

  static Ref<SubTexture2D> CreateFromCoords(
      const Ref<Texture2D>& texture, const glm::vec2& coords,
      const glm::vec2& cell_size, const glm::vec2& sprite_size = {1, 1});

 private:
  Ref<Texture2D> texture_;

  std::array<glm::vec2, 4> tex_coords_;
};

}  // namespace solar
