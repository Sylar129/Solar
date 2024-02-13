// Copyright (c) 2024 Sylar129

#pragma once

#include <glm/vec2.hpp>

#include "Core/Base/Sundry.h"
#include "Core/Renderer/Texture.h"

namespace Solar {

class SubTexture2D {
 public:
  SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min,
               const glm::vec2& max);

  Ref<Texture2D> GetTexture() const { return texture_; }
  const glm::vec2* GetTexCoords() const { return tex_coords_; }

  static Ref<SubTexture2D> CreateFromCoords(
      const Ref<Texture2D>& texture, const glm::vec2& coords,
      const glm::vec2& cell_size, const glm::vec2& sprite_size = {1, 1});

 private:
  Ref<Texture2D> texture_;

  glm::vec2 tex_coords_[4];
};

}  // namespace Solar
