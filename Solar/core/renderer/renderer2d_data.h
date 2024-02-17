// Copyright (c) 2024 Sylar129

#pragma once

#include "core/renderer/shader.h"
#include "core/renderer/texture.h"
#include "core/renderer/vertex_array.h"

namespace solar {

constexpr int kQuadVertexCount = 4;
constexpr int kQuadIndexCount = 6;

constexpr uint32_t kMaxQuads = 10000;
constexpr uint32_t kMaxVertices = kMaxQuads * kQuadVertexCount;
constexpr uint32_t kMaxIndices = kMaxQuads * kQuadIndexCount;
constexpr uint32_t kMaxTextureSlots = 32;  // TODO(sylar): Render capability

// Stats
struct Statistics {
  uint32_t draw_calls = 0;
  uint32_t quad_count = 0;

  uint32_t GetTotalVertexCount() const { return quad_count * kQuadVertexCount; }
  uint32_t GetTotalIndexCount() const { return quad_count * kQuadIndexCount; }
};

struct QuadVertex {
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 tex_coord;
  uint32_t tex_index;
  float tiling_factor;

  // Editor-only
  int entity_id;
};

struct Renderer2DData {
  Ref<VertexArray> quad_vertex_array;
  Ref<VertexBuffer> quad_vertex_buffer;
  Ref<Shader> texture_shader;
  Ref<Texture2D> white_texture;

  uint32_t quad_index_count = 0;
  QuadVertex* quad_vertex_buffer_base = nullptr;
  QuadVertex* quad_vertex_buffer_ptr = nullptr;

  std::array<Ref<Texture2D>, kMaxTextureSlots> texture_slots;
  uint32_t texture_slot_index = 1;  // 0 = white texture

  std::array<glm::vec4, kQuadVertexCount> quad_vertex_positions;

  Statistics stats;

  uint32_t DataSize() const {
    auto data_size = reinterpret_cast<uint8_t*>(quad_vertex_buffer_ptr) -
                     reinterpret_cast<uint8_t*>(quad_vertex_buffer_base);
    return data_size;
  }

  void Setup(const glm::mat4& transform, const glm::vec4& color,
             const std::array<glm::vec2, 4>& texture_coords,
             uint32_t texture_index, float tiling_factor, int entity_id) {
    for (int i = 0; i < kQuadVertexCount; ++i) {
      quad_vertex_buffer_ptr->position = transform * quad_vertex_positions[i];
      quad_vertex_buffer_ptr->color = color;
      quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
      quad_vertex_buffer_ptr->tex_index = texture_index;
      quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
      quad_vertex_buffer_ptr->entity_id = entity_id;
      quad_vertex_buffer_ptr++;
    }
    quad_index_count += kQuadIndexCount;

    stats.quad_count++;
  }
};

}  // namespace solar
