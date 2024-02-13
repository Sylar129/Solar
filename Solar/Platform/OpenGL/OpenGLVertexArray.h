// Copyright (c) 2024 Sylar129

#pragma once

#include <vector>

#include "Core/Renderer/VertexArray.h"

namespace Solar {

class OpenGLVertexArray : public VertexArray {
 public:
  OpenGLVertexArray();
  ~OpenGLVertexArray() override;

  void Bind() const override;
  void Unbind() const override;

  void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
  void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

  const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override;
  const Ref<IndexBuffer>& GetIndexBuffer() const override;

 private:
  uint32_t renderer_id_;
  uint32_t vertex_buffer_index_ = 0;
  std::vector<Ref<VertexBuffer>> vertex_buffers_;
  Ref<IndexBuffer> index_buffer_;
};

}  // namespace Solar
