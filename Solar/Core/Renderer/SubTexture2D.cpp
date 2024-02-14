// Copyright (c) 2024 Sylar129

#include "Core/Renderer/SubTexture2D.h"

namespace solar {

SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min,
                           const glm::vec2& max)
    : texture_(texture) {
  tex_coords_[0] = {min.x, min.y};
  tex_coords_[1] = {max.x, min.y};
  tex_coords_[2] = {max.x, max.y};
  tex_coords_[3] = {min.x, max.y};
}

Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture,
                                                 const glm::vec2& coords,
                                                 const glm::vec2& cell_size,
                                                 const glm::vec2& sprite_size) {
  auto sheet_width = texture->GetWidth();
  auto sheet_height = texture->GetHeight();

  glm::vec2 min = {(coords.x * cell_size.x) / sheet_width,
                   (coords.y * cell_size.y) / sheet_height};
  glm::vec2 max = {((coords.x + sprite_size.x) * cell_size.x) / sheet_width,
                   ((coords.y + sprite_size.y) * cell_size.y) / sheet_height};

  return CreateRef<SubTexture2D>(texture, min, max);
}

}  // namespace solar
