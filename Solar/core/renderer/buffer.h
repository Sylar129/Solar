// Copyright (c) 2024 Sylar129

#pragma once

#include <string>
#include <vector>

#include "core/renderer/shader_data_type.h"
#include "core/utils/misc.h"

namespace solar {

struct BufferElement {
  std::string name;
  ShaderDataType type;
  uint32_t size;
  uint32_t offset;
  bool normalized;

  BufferElement()
      : type(ShaderDataType::kNone), size(0), offset(0), normalized(false) {}

  BufferElement(ShaderDataType type, const std::string& name,
                bool normalized = false)
      : name(name),
        type(type),
        size(ShaderDataTypeSize(type)),
        offset(0),
        normalized(normalized) {}

  uint32_t GetComponentCount() const { return ShaderDataTypeCount(type); }
};

class BufferLayout {
 public:
  BufferLayout() : stride_(0) {}

  BufferLayout(const std::initializer_list<BufferElement>& elements)
      : elements_(elements), stride_(0) {
    CalculateOffsetAndStride();
  }

  inline uint32_t GetStride() const { return stride_; }
  inline const std::vector<BufferElement>& GetElements() const {
    return elements_;
  }

  auto begin() { return elements_.begin(); }
  auto begin() const { return elements_.cbegin(); }
  auto end() { return elements_.end(); }
  auto end() const { return elements_.cend(); }

 private:
  void CalculateOffsetAndStride() {
    uint32_t offset = 0;
    stride_ = 0;
    for (auto& element : elements_) {
      element.offset = offset;
      offset += element.size;
      stride_ += element.size;
    }
  }

  std::vector<BufferElement> elements_;
  uint32_t stride_;
};

class VertexBuffer {
 public:
  virtual ~VertexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetData(const void* data, uint32_t size) = 0;

  virtual const BufferLayout& GetLayout() const = 0;
  virtual void SetLayout(const BufferLayout& layout) = 0;

  static Ref<VertexBuffer> Create(uint32_t size);
  static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
};

class IndexBuffer {
 public:
  virtual ~IndexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual uint32_t GetCount() const = 0;

  static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
};

}  // namespace solar
