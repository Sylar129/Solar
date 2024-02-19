// Copyright (c) 2024 Sylar129

#include "core/utils/image.h"

#include "core/debug/assert.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // NOLINT

namespace solar {
Image::Image(const std::string& file_path, FlipVerticallyOnLoad filp_vertical)
    : width_(0), height_(0), channels_(0), data_(nullptr) {
  stbi_set_flip_vertically_on_load(static_cast<int>(filp_vertical));
  // 4 is hard-code right now. So is data format and internal format
  data_ = stbi_load(file_path.c_str(), &width_, &height_, &channels_, 4);
  if (data_ == nullptr) {
    SOLAR_ASSERT(false, "Fail to load image at {} \nFailure Reason: {}",
                 file_path, stbi_failure_reason());
  }
}

Image::~Image() { stbi_image_free(data_); }

int Image::GetWidth() const { return width_; }

int Image::GetHeight() const { return height_; }

int Image::GetChannels() const { return channels_; }

uint8_t* Image::GetData() const { return data_; }

}  // namespace solar
