// Copyright (c) 2024 Sylar129

#include "Core/Renderer/Renderer2D.h"

#include <cstdint>

#include "Core/Debug/Instrumentor.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/VertexArray.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Solar {

struct QuadVertex {
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 tex_coord;
  // TODO(sylar): why float?
  float tex_index;
  float tiling_factor;

  // Editor-only
  int entity_id;
};

struct Renderer2DData {
  static constexpr uint32_t kMaxQuads = 10000;
  static constexpr uint32_t kMaxVertices = kMaxQuads * 4;
  static constexpr uint32_t kMaxIndices = kMaxQuads * 6;
  static constexpr uint32_t kMaxTextureSlots =
      32;  // TODO(sylar): Render capability

  Ref<VertexArray> quad_vertex_array;
  Ref<VertexBuffer> quad_vertex_buffer;
  Ref<Shader> texture_shader;
  Ref<Texture2D> white_texture;

  uint32_t quad_index_count = 0;
  QuadVertex* quad_vertex_buffer_base = nullptr;
  QuadVertex* quad_vertex_buffer_ptr = nullptr;

  std::array<Ref<Texture2D>, kMaxTextureSlots> texture_slots;
  uint32_t texture_slot_index = 1;  // 0 = white texture

  glm::vec4 quad_vertex_positions[4];

  Renderer2D::Statistics stats;
};

static Renderer2DData s_data;

void Renderer2D::Init() {
  SOLAR_PROFILE_FUNCTION();

  s_data.quad_vertex_array = Solar::VertexArray::Create();

  s_data.quad_vertex_buffer =
      VertexBuffer::Create(s_data.kMaxVertices * sizeof(QuadVertex));
  s_data.quad_vertex_buffer->SetLayout(
      {{ShaderDataType::kFloat3, "a_Position"},
       {ShaderDataType::kFloat4, "a_Color"},
       {ShaderDataType::kFloat2, "a_TexCoord"},
       {ShaderDataType::kFloat, "a_TexIndex"},
       {ShaderDataType::kFloat, "a_tiling_factor"},
       {ShaderDataType::kInt, "a_EntityID"}});
  s_data.quad_vertex_array->AddVertexBuffer(s_data.quad_vertex_buffer);

  s_data.quad_vertex_buffer_base = new QuadVertex[s_data.kMaxVertices];

  uint32_t* quad_indices = new uint32_t[s_data.kMaxIndices];

  uint32_t offset = 0;
  for (auto i{0}; i < s_data.kMaxIndices; i += 6) {
    quad_indices[i + 0] = offset + 0;
    quad_indices[i + 1] = offset + 1;
    quad_indices[i + 2] = offset + 2;

    quad_indices[i + 3] = offset + 2;
    quad_indices[i + 4] = offset + 3;
    quad_indices[i + 5] = offset + 0;

    offset += 4;
  }
  Ref<IndexBuffer> quad_ib =
      IndexBuffer::Create(quad_indices, s_data.kMaxIndices);
  s_data.quad_vertex_array->SetIndexBuffer(quad_ib);
  delete[] quad_indices;

  s_data.white_texture = Texture2D::Create(1, 1);
  uint32_t white_texture_data = 0xffffffff;
  s_data.white_texture->SetData(&white_texture_data,
                                sizeof(white_texture_data));

  int32_t samplers[s_data.kMaxTextureSlots];
  for (auto i{0}; i < s_data.kMaxTextureSlots; ++i) {
    samplers[i] = i;
  }

  s_data.texture_shader = Shader::Create("assets/shaders/Texture.glsl");
  s_data.texture_shader->Bind();
  s_data.texture_shader->SetIntArray("u_Texture", samplers,
                                     s_data.kMaxTextureSlots);

  s_data.texture_slots[0] = s_data.white_texture;

  s_data.quad_vertex_positions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
  s_data.quad_vertex_positions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
  s_data.quad_vertex_positions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
  s_data.quad_vertex_positions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}

void Renderer2D::ShutDown() {
  SOLAR_PROFILE_FUNCTION();

  delete[] s_data.quad_vertex_buffer_base;
}

void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
  SOLAR_PROFILE_FUNCTION();

  glm::mat4 view_proj = camera.GetProjection() * glm::inverse(transform);

  s_data.texture_shader->Bind();
  s_data.texture_shader->SetMat4("u_ViewProjection", view_proj);

  StartBatch();
}

void Renderer2D::BeginScene(const EditorCamera& camera) {
  SOLAR_PROFILE_FUNCTION();

  glm::mat4 view_proj = camera.GetViewProjection();

  s_data.texture_shader->Bind();
  s_data.texture_shader->SetMat4("u_ViewProjection", view_proj);

  StartBatch();
}

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
  SOLAR_PROFILE_FUNCTION();

  s_data.texture_shader->Bind();
  s_data.texture_shader->SetMat4("u_ViewProjection",
                                 camera.GetViewProjectionMatrix());

  StartBatch();
}

void Renderer2D::EndScene() {
  SOLAR_PROFILE_FUNCTION();

  Flush();
}

void Renderer2D::Flush() {
  if (0 == s_data.quad_index_count) {
    return;
  }

  uint32_t data_size = (uint32_t)((uint8_t*)s_data.quad_vertex_buffer_ptr -
                                  (uint8_t*)s_data.quad_vertex_buffer_base);
  s_data.quad_vertex_buffer->SetData(s_data.quad_vertex_buffer_base, data_size);

  // Bind textures
  for (auto i{0u}; i < s_data.texture_slot_index; ++i) {
    s_data.texture_slots[i]->Bind(i);
  }

  RenderCommand::DrawIndexed(s_data.quad_vertex_array, s_data.quad_index_count);
  s_data.stats.draw_calls++;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                          const glm::vec4& color) {
  DrawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                          const glm::vec4& color) {
  SOLAR_PROFILE_FUNCTION();

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

  DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                          const Ref<Texture2D>& texture, float tiling_factor,
                          const glm::vec4& tint_color) {
  DrawQuad({position.x, position.y, 0.0f}, size, texture, tiling_factor,
           tint_color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                          const Ref<Texture2D>& texture, float tiling_factor,
                          const glm::vec4& tint_color) {
  SOLAR_PROFILE_FUNCTION();

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

  DrawQuad(transform, texture, tiling_factor, tint_color);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                          const Ref<SubTexture2D>& sub_texture,
                          float tiling_factor, const glm::vec4& tint_color) {
  DrawQuad({position.x, position.y, 0.0f}, size, sub_texture, tiling_factor,
           tint_color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                          const Ref<SubTexture2D>& sub_texture,
                          float tiling_factor, const glm::vec4& tint_color) {
  SOLAR_PROFILE_FUNCTION();

  if (s_data.quad_index_count >= s_data.kMaxIndices) {
    NextBatch();
  }

  constexpr size_t kQuadVertexCount = 4;
  constexpr glm::vec4 kDefaultColor = {1.0f, 1.0f, 1.0f, 1.0f};
  const glm::vec2* texture_coords = sub_texture->GetTexCoords();
  const Ref<Texture2D> texture = sub_texture->GetTexture();

  uint32_t texture_index = 0;

  for (auto i{1u}; i < s_data.texture_slot_index; i++) {
    if (*s_data.texture_slots[i].get() == *texture.get()) {
      texture_index = i;
      break;
    }
  }

  if (texture_index == 0) {
    if (s_data.texture_slot_index >= Renderer2DData::kMaxTextureSlots) {
      NextBatch();
    }

    texture_index = s_data.texture_slot_index;
    s_data.texture_slots[s_data.texture_slot_index] = texture;
    s_data.texture_slot_index++;
  }

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                        glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

  for (auto i{0}; i < kQuadVertexCount; ++i) {
    s_data.quad_vertex_buffer_ptr->position =
        transform * s_data.quad_vertex_positions[i];
    s_data.quad_vertex_buffer_ptr->color = kDefaultColor;
    s_data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
    s_data.quad_vertex_buffer_ptr->tex_index =
        static_cast<float>(texture_index);
    s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
    s_data.quad_vertex_buffer_ptr++;
  }

  s_data.quad_index_count += 6;

  s_data.stats.quad_count++;
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                          int entity_id) {
  SOLAR_PROFILE_FUNCTION();

  if (s_data.quad_index_count >= s_data.kMaxIndices) {
    NextBatch();
  }

  constexpr float kTextureIndex = 0.0f;  // White texture
  constexpr float kTilingFactor = 1.0f;
  constexpr size_t kQuadVertexCount = 4;
  constexpr glm::vec2 kTextureCoords[] = {
      {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  for (auto i{0}; i < kQuadVertexCount; ++i) {
    s_data.quad_vertex_buffer_ptr->position =
        transform * s_data.quad_vertex_positions[i];
    s_data.quad_vertex_buffer_ptr->color = color;
    s_data.quad_vertex_buffer_ptr->tex_coord = kTextureCoords[i];
    s_data.quad_vertex_buffer_ptr->tex_index = kTextureIndex;
    s_data.quad_vertex_buffer_ptr->tiling_factor = kTilingFactor;
    s_data.quad_vertex_buffer_ptr->entity_id = entity_id;
    s_data.quad_vertex_buffer_ptr++;
  }

  s_data.quad_index_count += 6;

  s_data.stats.quad_count++;
}

void Renderer2D::DrawQuad(const glm::mat4& transform,
                          const Ref<Texture2D>& texture, float tiling_factor,
                          const glm::vec4& tint_color, int entity_id) {
  SOLAR_PROFILE_FUNCTION();

  if (s_data.quad_index_count >= s_data.kMaxIndices) {
    NextBatch();
  }

  constexpr size_t kQuadVertexCount = 4;
  constexpr glm::vec4 kDefaultColor = {1.0f, 1.0f, 1.0f, 1.0f};
  constexpr glm::vec2 kTextureCoords[] = {
      {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  uint32_t texture_index;

  for (auto i{1u}; i < s_data.texture_slot_index; i++) {
    if (*s_data.texture_slots[i].get() == *texture.get()) {
      texture_index = i;
      break;
    }
  }

  if (texture_index == 0) {
    if (s_data.texture_slot_index >= Renderer2DData::kMaxTextureSlots) {
      NextBatch();
    }

    texture_index = s_data.texture_slot_index;
    s_data.texture_slots[s_data.texture_slot_index] = texture;
    s_data.texture_slot_index++;
  }

  for (auto i{0}; i < kQuadVertexCount; ++i) {
    s_data.quad_vertex_buffer_ptr->position =
        transform * s_data.quad_vertex_positions[i];
    s_data.quad_vertex_buffer_ptr->color = kDefaultColor;
    s_data.quad_vertex_buffer_ptr->tex_coord = kTextureCoords[i];
    s_data.quad_vertex_buffer_ptr->tex_index =
        static_cast<float>(texture_index);
    s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
    s_data.quad_vertex_buffer_ptr->entity_id = entity_id;
    s_data.quad_vertex_buffer_ptr++;
  }

  s_data.quad_index_count += 6;

  s_data.stats.quad_count++;
}

void Renderer2D::DrawRotateQuad(const glm::vec2& position,
                                const glm::vec2& size, float rotation,
                                const glm::vec4& color) {
  DrawRotateQuad({position.x, position.y, 0.0f}, size, rotation, color);
}

void Renderer2D::DrawRotateQuad(const glm::vec3& position,
                                const glm::vec2& size, float rotation,
                                const glm::vec4& color) {
  SOLAR_PROFILE_FUNCTION();

  if (s_data.quad_index_count >= s_data.kMaxIndices) {
    NextBatch();
  }

  constexpr float kTextureIndex = 0.0f;  // White texture
  constexpr float kTilingFactor = 1.0f;
  constexpr size_t kQuadVertexCount = 4;
  constexpr glm::vec2 kTextureCoords[] = {
      {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
      glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

  for (auto i{0}; i < kQuadVertexCount; ++i) {
    s_data.quad_vertex_buffer_ptr->position =
        transform * s_data.quad_vertex_positions[i];
    s_data.quad_vertex_buffer_ptr->color = color;
    s_data.quad_vertex_buffer_ptr->tex_coord = kTextureCoords[i];
    s_data.quad_vertex_buffer_ptr->tex_index = kTextureIndex;
    s_data.quad_vertex_buffer_ptr->tiling_factor = kTilingFactor;
    s_data.quad_vertex_buffer_ptr++;
  }

  s_data.quad_index_count += 6;

  s_data.stats.quad_count++;
}

void Renderer2D::DrawRotateQuad(const glm::vec2& position,
                                const glm::vec2& size, float rotation,
                                const Ref<Texture2D>& texture,
                                float tiling_factor,
                                const glm::vec4& tint_color) {
  DrawRotateQuad({position.x, position.y, 0.0f}, size, rotation, texture,
                 tiling_factor, tint_color);
}

void Renderer2D::DrawRotateQuad(const glm::vec3& position,
                                const glm::vec2& size, float rotation,
                                const Ref<Texture2D>& texture,
                                float tiling_factor,
                                const glm::vec4& tint_color) {
  SOLAR_PROFILE_FUNCTION();

  if (s_data.quad_index_count >= s_data.kMaxIndices) {
    NextBatch();
  }

  constexpr size_t kQuadVertexCount = 4;
  constexpr glm::vec4 kDefaultColor = {1.0f, 1.0f, 1.0f, 1.0f};
  constexpr glm::vec2 kTextureCoords[] = {
      {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  uint32_t texture_index = 0;

  for (auto i{1u}; i < s_data.texture_slot_index; i++) {
    if (*s_data.texture_slots[i].get() == *texture.get()) {
      texture_index = i;
      break;
    }
  }

  if (texture_index == 0.0f) {
    if (s_data.texture_slot_index >= Renderer2DData::kMaxTextureSlots) {
      NextBatch();
    }

    texture_index = s_data.texture_slot_index;
    s_data.texture_slots[s_data.texture_slot_index] = texture;
    s_data.texture_slot_index++;
  }

  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
      glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

  for (auto i{0}; i < kQuadVertexCount; ++i) {
    s_data.quad_vertex_buffer_ptr->position =
        transform * s_data.quad_vertex_positions[i];
    s_data.quad_vertex_buffer_ptr->color = kDefaultColor;
    s_data.quad_vertex_buffer_ptr->tex_coord = kTextureCoords[i];
    s_data.quad_vertex_buffer_ptr->tex_index =
        static_cast<float>(texture_index);
    s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
    s_data.quad_vertex_buffer_ptr++;
  }

  s_data.quad_index_count += 6;

  s_data.stats.quad_count++;
}

void Renderer2D::DrawRotateQuad(const glm::vec2& position,
                                const glm::vec2& size, float rotation,
                                const Ref<SubTexture2D>& sub_texture,
                                float tiling_factor,
                                const glm::vec4& tint_color) {
  DrawRotateQuad({position.x, position.y, 0.0f}, size, rotation, sub_texture,
                 tiling_factor, tint_color);
}

void Renderer2D::DrawRotateQuad(const glm::vec3& position,
                                const glm::vec2& size, float rotation,
                                const Ref<SubTexture2D>& sub_texture,
                                float tiling_factor,
                                const glm::vec4& tint_color) {
  SOLAR_PROFILE_FUNCTION();

  if (s_data.quad_index_count >= s_data.kMaxIndices) {
    NextBatch();
  }

  constexpr size_t kQuadVertexCount = 4;
  constexpr glm::vec4 kDefaultColor = {1.0f, 1.0f, 1.0f, 1.0f};
  const glm::vec2* texture_coords = sub_texture->GetTexCoords();
  const Ref<Texture2D> texture = sub_texture->GetTexture();

  uint32_t texture_index = 0;

  for (auto i{1u}; i < s_data.texture_slot_index; i++) {
    if (*s_data.texture_slots[i].get() == *texture.get()) {
      texture_index = i;
      break;
    }
  }

  if (texture_index == 0) {
    if (s_data.texture_slot_index >= Renderer2DData::kMaxTextureSlots) {
      NextBatch();
    }

    texture_index = s_data.texture_slot_index;
    s_data.texture_slots[s_data.texture_slot_index] = texture;
    s_data.texture_slot_index++;
  }

  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
      glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

  for (auto i{0}; i < kQuadVertexCount; ++i) {
    s_data.quad_vertex_buffer_ptr->position =
        transform * s_data.quad_vertex_positions[i];
    s_data.quad_vertex_buffer_ptr->color = kDefaultColor;
    s_data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
    s_data.quad_vertex_buffer_ptr->tex_index =
        static_cast<float>(texture_index);
    s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
    s_data.quad_vertex_buffer_ptr++;
  }

  s_data.quad_index_count += 6;

  s_data.stats.quad_count++;
}

void Renderer2D::DrawSprite(const glm::mat4& transform,
                            const SpriteRendererComponent& src, int entity_id) {
  DrawQuad(transform, src.color, entity_id);
}

void Renderer2D::ResetStats() {
  memset(&s_data.stats, 0, sizeof(s_data.stats));
}

Renderer2D::Statistics Renderer2D::GetStats() { return s_data.stats; }

void Renderer2D::StartBatch() {
  s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
  s_data.quad_index_count = 0;
  s_data.texture_slot_index = 1;
}

void Renderer2D::NextBatch() {
  Flush();
  StartBatch();
}

}  // namespace Solar
