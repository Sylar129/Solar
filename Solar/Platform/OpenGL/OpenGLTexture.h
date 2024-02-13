// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Renderer/Texture.h"
#include "glad/glad.h"

namespace Solar {

class OpenGLTexture2D : public Texture2D {
 public:
  OpenGLTexture2D(uint32_t width, uint32_t height);
  explicit OpenGLTexture2D(const std::string& path);
  virtual ~OpenGLTexture2D();

  virtual uint32_t GetWidth() const override { return width_; }
  virtual uint32_t GetHeight() const override { return height_; }
  virtual uint32_t GetRendererID() const override { return renderer_id_; }

  virtual void SetData(void* data, uint32_t size) override;

  virtual void Bind(uint32_t slot) const override;

  virtual bool operator==(const Texture& other) const override {
    return renderer_id_ == ((OpenGLTexture2D&)other).renderer_id_;
  }

 private:
  std::string path_;
  uint32_t width_;
  uint32_t height_;
  uint32_t renderer_id_;
  GLenum internale_format_;
  GLenum data_format_;
};

}  // namespace Solar
