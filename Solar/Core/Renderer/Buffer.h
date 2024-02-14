// Copyright (c) 2024 Sylar129

#pragma once

#include <string>
#include <vector>

#include "core/base/sundry.h"

namespace solar {

enum class ShaderDataType {
  kNone = 0,
  kFloat,
  kFloat2,
  kFloat3,
  kFloat4,
  kMat3,
  kMat4,
  kInt,
  kInt2,
  kInt3,
  kInt4,
  kBool
};

constexpr uint32_t ShaderDataTypeSize(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::kFloat:
      return 4;
    case ShaderDataType::kFloat2:
      return 4 * 2;
    case ShaderDataType::kFloat3:
      return 4 * 3;
    case ShaderDataType::kFloat4:
      return 4 * 4;
    case ShaderDataType::kMat3:
      return 4 * 3 * 3;
    case ShaderDataType::kMat4:
      return 4 * 4 * 4;
    case ShaderDataType::kInt:
      return 4;
    case ShaderDataType::kInt2:
      return 4 * 2;
    case ShaderDataType::kInt3:
      return 4 * 3;
    case ShaderDataType::kInt4:
      return 4 * 4;
    case ShaderDataType::kBool:
      return 1;
    case ShaderDataType::kNone:
      break;
  }

  SOLAR_CORE_ASSERT(false, "Unknown Shader Data Type!");
  return 0;
}

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

  uint32_t GetComponentCount() const {
    switch (type) {
      case ShaderDataType::kFloat:
        return 1;
      case ShaderDataType::kFloat2:
        return 2;
      case ShaderDataType::kFloat3:
        return 3;
      case ShaderDataType::kFloat4:
        return 4;
      case ShaderDataType::kMat3:
        return 3 * 3;
      case ShaderDataType::kMat4:
        return 4 * 4;
      case ShaderDataType::kInt:
        return 1;
      case ShaderDataType::kInt2:
        return 2;
      case ShaderDataType::kInt3:
        return 3;
      case ShaderDataType::kInt4:
        return 4;
      case ShaderDataType::kBool:
        return 1;
      case ShaderDataType::kNone:
        break;
    }
    SOLAR_CORE_ASSERT(false, "Unknown Shader Data Type!");
    return 0;
  }
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

  std::vector<BufferElement>::iterator begin() { return elements_.begin(); }
  std::vector<BufferElement>::iterator end() { return elements_.end(); }
  std::vector<BufferElement>::const_iterator begin() const {
    return elements_.begin();
  }
  std::vector<BufferElement>::const_iterator end() const {
    return elements_.end();
  }

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
