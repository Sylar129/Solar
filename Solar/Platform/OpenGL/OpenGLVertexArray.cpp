#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

#include "Core/Debug/Instrumentor.h"

namespace Solar {

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

  glCreateVertexArrays(1, &m_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
  SOLAR_PROFILE_FUNCTION();

  glDeleteVertexArrays(1, &m_RendererID);
}

void OpenGLVertexArray::Bind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindVertexArray(m_RendererID);
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

  glBindVertexArray(m_RendererID);
  vertex_buffer->Bind();

  const auto& layout = vertex_buffer->GetLayout();
  for (const auto& element : layout) {
    switch (element.type) {
      case ShaderDataType::kFloat:
      case ShaderDataType::kFloat2:
      case ShaderDataType::kFloat3:
      case ShaderDataType::kFloat4: {
        glEnableVertexAttribArray(m_VertexBufferIndex);
        glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(), (const void*)element.offset);
        m_VertexBufferIndex++;
      } break;
      case ShaderDataType::kInt:
      case ShaderDataType::kInt2:
      case ShaderDataType::kInt3:
      case ShaderDataType::kInt4:
      case ShaderDataType::kBool:
        glEnableVertexAttribArray(m_VertexBufferIndex);
        glVertexAttribIPointer(m_VertexBufferIndex, element.GetComponentCount(),
                               ShaderDataTypeToOpenGLBaseType(element.type),
                               layout.GetStride(), (const void*)element.offset);
        m_VertexBufferIndex++;
        break;
      case ShaderDataType::kMat3:
      case ShaderDataType::kMat4: {
        auto count = element.GetComponentCount();
        for (auto i{0u}; i < count; i++) {
          glEnableVertexAttribArray(m_VertexBufferIndex);
          glVertexAttribPointer(
              m_VertexBufferIndex, count,
              ShaderDataTypeToOpenGLBaseType(element.type),
              element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
              (const void*)(element.offset + sizeof(float) * count * i));
          glVertexAttribDivisor(m_VertexBufferIndex, 1);
          m_VertexBufferIndex++;
        }
      } break;
      default:
        SOLAR_CORE_ASSERT(false, "Unknown ShaderDataType!");
    }
  }
  m_VertexBuffers.push_back(vertex_buffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) {
  SOLAR_PROFILE_FUNCTION();

  glBindVertexArray(m_RendererID);
  index_buffer->Bind();

  m_IndexBuffer = index_buffer;
}

const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers()
    const {
  return m_VertexBuffers;
}

const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const {
  return m_IndexBuffer;
}

}  // namespace Solar
