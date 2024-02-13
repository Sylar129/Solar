#pragma once

#include "Core/Renderer/Buffer.h"

namespace Solar {

class OpenGLVertexBuffer : public VertexBuffer {
 public:
  explicit OpenGLVertexBuffer(uint32_t size);
  OpenGLVertexBuffer(float* vertices, uint32_t size);
  virtual ~OpenGLVertexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void SetData(const void* data, uint32_t size) override;

  virtual const BufferLayout& GetLayout() const override;
  virtual void SetLayout(const BufferLayout& layout) override;

 private:
  uint32_t renderer_id_;
  BufferLayout layout_;
};

class OpenGLIndexBuffer : public IndexBuffer {
 public:
  OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
  virtual ~OpenGLIndexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  inline virtual uint32_t GetCount() const override { return count_; }

 private:
  uint32_t renderer_id_;
  uint32_t count_;
};
}  // namespace Solar
