// Copyright (c) 2024 Sylar129

#pragma once

#include "core/debug/assert.h"

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

constexpr uint32_t ShaderDataTypeCount(ShaderDataType type) {
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

}  // namespace solar
