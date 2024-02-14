// Copyright (c) 2024 Sylar129

#include "platform/opengl/opengl_texture.h"

#include "core/debug/instrumentor.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // NOLINT

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

  int width;
  int height;
  int channels;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = nullptr;
  {
    SOLAR_PROFILE_SCOPE("OpenGLTexture2D(stbi_load)");
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  }
  SOLAR_CORE_ASSERT(data, "Failed to load image!");
  width_ = width;
  height_ = height;

  GLenum internal_format = 0;
  GLenum data_format = 0;

  if (channels == 4) {
    internal_format = GL_RGBA8;
    data_format = GL_RGBA;
  } else if (channels == 3) {
    internal_format = GL_RGB8;
    data_format = GL_RGB;
  }

  internale_format_ = internal_format;
  data_format_ = data_format;

  SOLAR_CORE_ASSERT(internal_format && data_format, "Format not supported!!");

  glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
  glTextureStorage2D(renderer_id_, 1, internal_format, width_, height_);

  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTextureSubImage2D(renderer_id_, 0, 0, 0, width_, height_, data_format,
                      GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {
  SOLAR_PROFILE_FUNCTION();

  glDeleteTextures(1, &renderer_id_);
}

void OpenGLTexture2D::SetData(void* data, uint32_t size) {
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
