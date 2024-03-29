// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "core/utils/misc.h"

namespace solar {

class Texture {
 public:
  virtual ~Texture() = default;

  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;
  virtual uint32_t GetRendererID() const = 0;

  virtual void SetData(const void* data, uint32_t size) = 0;

  virtual void Bind(uint32_t slot) const = 0;

  virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture {
 public:
  static Ref<Texture2D> Create(uint32_t width, uint32_t height);
  static Ref<Texture2D> Create(const std::string& path);
};

}  // namespace solar
