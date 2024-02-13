#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

#include "Core/Debug/Instrumentor.h"

namespace Solar {

// OpenGLVertexBuffer
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
  SOLAR_PROFILE_FUNCTION();

  glCreateBuffers(1, &renderer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
  SOLAR_PROFILE_FUNCTION();

  glCreateBuffers(1, &renderer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  SOLAR_PROFILE_FUNCTION();

  glDeleteBuffers(1, &renderer_id_);
}

void OpenGLVertexBuffer::Bind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
}

void OpenGLVertexBuffer::Unbind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

const BufferLayout& OpenGLVertexBuffer::GetLayout() const {
  SOLAR_PROFILE_FUNCTION();

  return layout_;
}

void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout) {
  SOLAR_PROFILE_FUNCTION();

  layout_ = layout;
}

// OpenGLIndexBuffer
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : count_(count) {
  SOLAR_PROFILE_FUNCTION();

  glCreateBuffers(1, &renderer_id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  SOLAR_PROFILE_FUNCTION();

  glDeleteBuffers(1, &renderer_id_);
}

void OpenGLIndexBuffer::Bind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
}

void OpenGLIndexBuffer::Unbind() const {
  SOLAR_PROFILE_FUNCTION();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}  // namespace Solar
