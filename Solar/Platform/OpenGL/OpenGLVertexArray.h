#pragma once

#include "Core/Renderer/VertexArray.h"

namespace Solar {

class OpenGLVertexArray : public VertexArray {
 public:
  OpenGLVertexArray();
  virtual ~OpenGLVertexArray();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
  virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

  virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers()
      const override;
  virtual const Ref<IndexBuffer>& GetIndexBuffer() const override;

 private:
  uint32_t renderer_id_;
  uint32_t vertex_buffer_index_ = 0;
  std::vector<Ref<VertexBuffer>> vertex_buffers_;
  Ref<IndexBuffer> index_buffer_;
};

}  // namespace Solar
