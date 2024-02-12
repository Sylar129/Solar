#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

#include "Core/Debug/Instrumentor.h"

namespace Solar {

namespace {
constexpr GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
      return GL_FLOAT;
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
      return GL_INT;
    case ShaderDataType::Bool:
      return GL_BOOL;
    case ShaderDataType::None:
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
    switch (element.Type) {
      case ShaderDataType::Float:
      case ShaderDataType::Float2:
      case ShaderDataType::Float3:
      case ShaderDataType::Float4: {
        glEnableVertexAttribArray(m_VertexBufferIndex);
        glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.Type),
                              element.Normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(), (const void*)element.Offset);
        m_VertexBufferIndex++;
      } break;
      case ShaderDataType::Int:
      case ShaderDataType::Int2:
      case ShaderDataType::Int3:
      case ShaderDataType::Int4:
      case ShaderDataType::Bool:
        glEnableVertexAttribArray(m_VertexBufferIndex);
        glVertexAttribIPointer(m_VertexBufferIndex, element.GetComponentCount(),
                               ShaderDataTypeToOpenGLBaseType(element.Type),
                               layout.GetStride(), (const void*)element.Offset);
        m_VertexBufferIndex++;
        break;
      case ShaderDataType::Mat3:
      case ShaderDataType::Mat4: {
        auto count = element.GetComponentCount();
        for (auto i{0u}; i < count; i++) {
          glEnableVertexAttribArray(m_VertexBufferIndex);
          glVertexAttribPointer(
              m_VertexBufferIndex, count,
              ShaderDataTypeToOpenGLBaseType(element.Type),
              element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
              (const void*)(element.Offset + sizeof(float) * count * i));
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
