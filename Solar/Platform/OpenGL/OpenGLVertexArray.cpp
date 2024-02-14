// Copyright (c) 2024 Sylar129

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Core/Debug/Instrumentor.h"
#include "glad/glad.h"

namespace solar {

namespace {
constexpr GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::kFloat:
    case ShaderDataType::kFloat2:
    case ShaderDataType::kFloat3:
    case ShaderDataType::kFloat4:
    case ShaderDataType::kMat3:
    case ShaderDataType::kMat4:
      return GL_FLOAT;
    case ShaderDataType::kInt:
    case ShaderDataType::kInt2:
    case ShaderDataType::kInt3:
    case ShaderDataType::kInt4:
      return GL_INT;
    case ShaderDataType::kBool:
      return GL_BOOL;
    case ShaderDataType::kNone:
      break;
  }
  SOLAR_CORE_ASSERT(false, "Unknown Shader Data Type!");
  return 0;
}
}  // namespace

OpenGLVertexArray::OpenGLVertexArray() {
  SOLAR_PROFILE_FUNCTION();

  glCreateVertexArrays(1, &renderer_id_);
}

OpenGLVertexArray::~OpenGLVertexArray() {
  SOLAR_PROFILE_FUNCTION();

  glDeleteVertexArrays(1, &renderer_id_);
}

void OpenGLVertexArray::Bind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindVertexArray(renderer_id_);
}

void OpenGLVertexArray::Unbind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(
    const Ref<VertexBuffer>& vertex_buffer) {
  SOLAR_PROFILE_FUNCTION();

  SOLAR_CORE_ASSERT(vertex_buffer->GetLayout().GetElements().size(),
                    "Vertex Buffer has no layout!");

  glBindVertexArray(renderer_id_);
  vertex_buffer->Bind();

  const auto& layout = vertex_buffer->GetLayout();
  for (const auto& element : layout) {
    switch (element.type) {
      case ShaderDataType::kFloat:
      case ShaderDataType::kFloat2:
      case ShaderDataType::kFloat3:
      case ShaderDataType::kFloat4: {
        glEnableVertexAttribArray(vertex_buffer_index_);
        glVertexAttribPointer(vertex_buffer_index_, element.GetComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(), (const void*)element.offset);
        vertex_buffer_index_++;
      } break;
      case ShaderDataType::kInt:
      case ShaderDataType::kInt2:
      case ShaderDataType::kInt3:
      case ShaderDataType::kInt4:
      case ShaderDataType::kBool:
        glEnableVertexAttribArray(vertex_buffer_index_);
        glVertexAttribIPointer(vertex_buffer_index_,
                               element.GetComponentCount(),
                               ShaderDataTypeToOpenGLBaseType(element.type),
                               layout.GetStride(), (const void*)element.offset);
        vertex_buffer_index_++;
        break;
      case ShaderDataType::kMat3:
      case ShaderDataType::kMat4: {
        auto count = element.GetComponentCount();
        for (auto i{0u}; i < count; i++) {
          glEnableVertexAttribArray(vertex_buffer_index_);
          glVertexAttribPointer(
              vertex_buffer_index_, count,
              ShaderDataTypeToOpenGLBaseType(element.type),
              element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
              (const void*)(element.offset + sizeof(float) * count * i));
          glVertexAttribDivisor(vertex_buffer_index_, 1);
          vertex_buffer_index_++;
        }
      } break;
      default:
        SOLAR_CORE_ASSERT(false, "Unknown ShaderDataType!");
    }
  }
  vertex_buffers_.push_back(vertex_buffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) {
  SOLAR_PROFILE_FUNCTION();

  glBindVertexArray(renderer_id_);
  index_buffer->Bind();

  index_buffer_ = index_buffer;
}

const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers()
    const {
  return vertex_buffers_;
}

const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const {
  return index_buffer_;
}

}  // namespace solar
