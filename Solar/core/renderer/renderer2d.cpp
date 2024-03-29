// Copyright (c) 2024 Sylar129

#include "core/renderer/renderer2d.h"

#include <cstdint>

#include "core/debug/instrumentor.h"
#include "core/renderer/render_command.h"
#include "core/renderer/renderer2d_data.h"
#include "core/renderer/shader.h"
#include "core/renderer/vertex_array.h"
#include "glm/fwd.hpp"

namespace solar {

namespace {
constexpr uint32_t kTextureIndex = 0;  // White texture
constexpr uint32_t kWhiteTextureData = 0xffffffff;

constexpr float kTilingFactor = 1;
constexpr std::array<glm::vec2, kQuadVertexCount> kTextureCoords = {
    glm::vec2{0, 0}, glm::vec2{1, 0}, glm::vec2{1, 1}, glm::vec2{0, 1}};

// TODO(sylar): move
Renderer2DData s_data;

}  // namespace

void Renderer2D::Init() {
  SOLAR_PROFILE_FUNCTION();

  s_data.quad_vertex_array = VertexArray::Create();

  s_data.quad_vertex_buffer =
      VertexBuffer::Create(kMaxVertices * sizeof(QuadVertex));
  s_data.quad_vertex_buffer->SetLayout(
      {{ShaderDataType::kFloat3, "a_Position"},
       {ShaderDataType::kFloat4, "a_Color"},
       {ShaderDataType::kFloat2, "a_TexCoord"},
       {ShaderDataType::kFloat, "a_TexIndex"},
       {ShaderDataType::kFloat, "a_tiling_factor"},
       {ShaderDataType::kInt, "a_EntityID"}});
  s_data.quad_vertex_array->AddVertexBuffer(s_data.quad_vertex_buffer);

  s_data.quad_vertex_buffer_base = new QuadVertex[kMaxVertices];

  uint32_t* quad_indices = new uint32_t[kMaxIndices];

  uint32_t offset = 0;
  for (auto i{0}; i < kMaxIndices; i += kQuadIndexCount) {
    quad_indices[i + 0] = offset + 0;
    quad_indices[i + 1] = offset + 1;
    quad_indices[i + 2] = offset + 2;

    quad_indices[i + 3] = offset + 2;
    quad_indices[i + 4] = offset + 3;
    quad_indices[i + 5] = offset + 0;

    offset += kQuadVertexCount;
  }
  Ref<IndexBuffer> quad_ib = IndexBuffer::Create(quad_indices, kMaxIndices);
  s_data.quad_vertex_array->SetIndexBuffer(quad_ib);
  delete[] quad_indices;

  s_data.white_texture = Texture2D::Create(1, 1);
  s_data.white_texture->SetData(&kWhiteTextureData, sizeof(kWhiteTextureData));

  int32_t samplers[kMaxTextureSlots];
  for (auto i{0}; i < kMaxTextureSlots; ++i) {
    samplers[i] = i;
  }

  s_data.texture_shader = Shader::Create("assets/shaders/Texture.glsl");
  s_data.texture_shader->Bind();
  s_data.texture_shader->SetIntArray("u_Texture", samplers, kMaxTextureSlots);

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

  glm::mat4 view_proj = camera.CalcViewProjection();

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

  s_data.quad_vertex_buffer->SetData(s_data.quad_vertex_buffer_base,
                                     s_data.DataSize());

  // Bind textures
  for (auto i{0U}; i < s_data.texture_slot_index; ++i) {
    s_data.texture_slots[i]->Bind(i);
  }

  RenderCommand::DrawIndexed(s_data.quad_vertex_array, s_data.quad_index_count);
  s_data.stats.draw_calls++;
}

// TODO(sylar): redundant
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                          const glm::vec4& color) {
  SOLAR_PROFILE_FUNCTION();

  glm::mat4 transform = glm::translate(glm::mat4(1), position) *
                        glm::scale(glm::mat4(1), {size.x, size.y, 1});

  DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                          int entity_id) {
  SOLAR_PROFILE_FUNCTION();

  if (s_data.quad_index_count >= kMaxIndices) {
    NextBatch();
  }

  s_data.Setup(transform, color, kTextureCoords, kTextureIndex, kTilingFactor,
               entity_id);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                          const glm::vec4& color, const Ref<Texture2D>& texture,
                          float tiling_factor) {
  SOLAR_PROFILE_FUNCTION();

  glm::mat4 transform = glm::translate(glm::mat4(1), position) *
                        glm::scale(glm::mat4(1), {size.x, size.y, 1});

  DrawQuad(transform, color, texture, tiling_factor);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                          const Ref<Texture2D>& texture, float tiling_factor,
                          int entity_id) {
  SOLAR_PROFILE_FUNCTION();

  if (s_data.quad_index_count >= kMaxIndices) {
    NextBatch();
  }

  uint32_t texture_index = 0;

  for (auto i{1U}; i < s_data.texture_slot_index; i++) {
    if (*s_data.texture_slots[i] == *texture) {
      texture_index = i;
      break;
    }
  }

  if (texture_index == 0) {
    if (s_data.texture_slot_index >= kMaxTextureSlots) {
      NextBatch();
    }

    texture_index = s_data.texture_slot_index;
    s_data.texture_slots[s_data.texture_slot_index] = texture;
    s_data.texture_slot_index++;
  }

  s_data.Setup(transform, color, kTextureCoords, texture_index, tiling_factor,
               entity_id);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                          const glm::vec4& color,
                          const Ref<SubTexture2D>& sub_texture,
                          float tiling_factor) {
  SOLAR_PROFILE_FUNCTION();

  glm::mat4 transform = glm::translate(glm::mat4(1), position) *
                        glm::scale(glm::mat4(1), {size.x, size.y, 1});

  DrawQuad(transform, color, sub_texture, tiling_factor);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                          const Ref<SubTexture2D>& sub_texture,
                          float tiling_factor) {
  SOLAR_PROFILE_FUNCTION();

  if (s_data.quad_index_count >= kMaxIndices) {
    NextBatch();
  }

  const auto& texture_coords = sub_texture->GetTexCoords();
  Ref<Texture2D> texture = sub_texture->GetTexture();

  uint32_t texture_index = 0;

  for (auto i{1U}; i < s_data.texture_slot_index; i++) {
    if (*s_data.texture_slots[i] == *texture) {
      texture_index = i;
      break;
    }
  }

  if (texture_index == 0) {
    if (s_data.texture_slot_index >= kMaxTextureSlots) {
      NextBatch();
    }

    texture_index = s_data.texture_slot_index;
    s_data.texture_slots[s_data.texture_slot_index] = texture;
    s_data.texture_slot_index++;
  }

  s_data.Setup(transform, color, texture_coords, texture_index, tiling_factor,
               -1);
}

void Renderer2D::DrawRotateQuad(const glm::vec3& position,
                                const glm::vec2& size, float rotation,
                                const glm::vec4& color) {
  SOLAR_PROFILE_FUNCTION();

  if (s_data.quad_index_count >= kMaxIndices) {
    NextBatch();
  }

  glm::mat4 transform =
      glm::translate(glm::mat4(1), position) *
      glm::rotate(glm::mat4(1), glm::radians(rotation), {0, 0, 1}) *
      glm::scale(glm::mat4(1), {size.x, size.y, 1});

  DrawQuad(transform, color);
}

void Renderer2D::DrawRotateQuad(const glm::vec3& position,
                                const glm::vec2& size, float rotation,
                                const glm::vec4& color,
                                const Ref<Texture2D>& texture,
                                float tiling_factor) {
  SOLAR_PROFILE_FUNCTION();

  glm::mat4 transform =
      glm::translate(glm::mat4(1), position) *
      glm::rotate(glm::mat4(1), glm::radians(rotation), {0, 0, 1}) *
      glm::scale(glm::mat4(1), {size.x, size.y, 1});

  DrawQuad(transform, color, texture, tiling_factor);
}

void Renderer2D::DrawRotateQuad(const glm::vec3& position,
                                const glm::vec2& size, float rotation,
                                const glm::vec4& color,
                                const Ref<SubTexture2D>& sub_texture,
                                float tiling_factor) {
  SOLAR_PROFILE_FUNCTION();

  glm::mat4 transform =
      glm::translate(glm::mat4(1), position) *
      glm::rotate(glm::mat4(1), glm::radians(rotation), {0, 0, 1}) *
      glm::scale(glm::mat4(1), {size.x, size.y, 1});

  DrawQuad(transform, color, sub_texture, tiling_factor);
}

void Renderer2D::DrawSprite(const TransformComponent& transform,
                            const SpriteRendererComponent& src, int entity_id) {
  DrawQuad(transform.GetTranform(), src.color, entity_id);
}

void Renderer2D::ResetStats() {
  memset(&s_data.stats, 0, sizeof(s_data.stats));
}

Statistics Renderer2D::GetStats() { return s_data.stats; }

void Renderer2D::StartBatch() {
  s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
  s_data.quad_index_count = 0;
  s_data.texture_slot_index = 1;
}

void Renderer2D::NextBatch() {
  Flush();
  StartBatch();
}

}  // namespace solar
