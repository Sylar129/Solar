#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

#include "Core/Debug/Instrumentor.h"

namespace Solar {

// OpenGLVertexBuffer
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
  SOLAR_PROFILE_FUNCTION();

  glCreateBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
  SOLAR_PROFILE_FUNCTION();

  glCreateBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  SOLAR_PROFILE_FUNCTION();

  glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

const BufferLayout& OpenGLVertexBuffer::GetLayout() const {
  SOLAR_PROFILE_FUNCTION();

  return m_Layout;
}

void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout) {
  SOLAR_PROFILE_FUNCTION();

  m_Layout = layout;
}

// OpenGLIndexBuffer
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : m_Count(count) {
  SOLAR_PROFILE_FUNCTION();

  glCreateBuffers(1, &m_RendererID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  SOLAR_PROFILE_FUNCTION();

  glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}  // namespace Solar
