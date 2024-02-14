// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Renderer/Buffer.h"

namespace solar {

class OpenGLVertexBuffer : public VertexBuffer {
 public:
  explicit OpenGLVertexBuffer(uint32_t size);
  OpenGLVertexBuffer(float* vertices, uint32_t size);
  ~OpenGLVertexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  void SetData(const void* data, uint32_t size) override;

  const BufferLayout& GetLayout() const override;
  void SetLayout(const BufferLayout& layout) override;

 private:
  uint32_t renderer_id_;
  BufferLayout layout_;
};

class OpenGLIndexBuffer : public IndexBuffer {
 public:
  OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
  ~OpenGLIndexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  inline uint32_t GetCount() const override { return count_; }

 private:
  uint32_t renderer_id_;
  uint32_t count_;
};
}  // namespace solar
