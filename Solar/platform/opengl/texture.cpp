// Copyright (c) 2024 Sylar129

#include "platform/opengl/texture.h"

#include "core/debug/assert.h"
#include "core/debug/instrumentor.h"
#include "core/utils/image.h"

namespace solar {

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : width_(width),
      height_(height),
      internale_format_(GL_RGBA8),
      data_format_(GL_RGBA) {
  SOLAR_PROFILE_FUNCTION();

  glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
  glTextureStorage2D(renderer_id_, 1, internale_format_, width_, height_);

  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path_(path) {
  SOLAR_PROFILE_FUNCTION();

  Image image(path, FlipVerticallyOnLoad::kFlip);
  width_ = image.GetWidth();
  height_ = image.GetHeight();

  // hard-code for now
  internale_format_ = GL_RGBA8;
  data_format_ = GL_RGBA;

  glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
  glTextureStorage2D(renderer_id_, 1, internale_format_, width_, height_);

  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTextureSubImage2D(renderer_id_, 0, 0, 0, width_, height_, data_format_,
                      GL_UNSIGNED_BYTE, image.GetData());
}

OpenGLTexture2D::~OpenGLTexture2D() {
  SOLAR_PROFILE_FUNCTION();

  glDeleteTextures(1, &renderer_id_);
}

void OpenGLTexture2D::SetData(const void* data, uint32_t size) {
  SOLAR_PROFILE_FUNCTION();

  uint32_t bytes_per_pixel = data_format_ == GL_RGBA ? 4 : 3;
  SOLAR_CORE_ASSERT(size == width_ * height_ * bytes_per_pixel,
                    "Data nust be entire texture!");
  glTextureSubImage2D(renderer_id_, 0, 0, 0, width_, height_, data_format_,
                      GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
  SOLAR_PROFILE_FUNCTION();

  glBindTextureUnit(slot, renderer_id_);
}

}  // namespace solar
