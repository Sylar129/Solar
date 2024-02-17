// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

namespace solar {

enum class FlipVerticallyOnLoad {
  kNoFlip = 0,
  kFlip,
};

class Image {
 public:
  explicit Image(const std::string& file_path,
                 FlipVerticallyOnLoad filp_vertical);
  ~Image();

  int GetWidth() const;
  int GetHeight() const;
  int GetChannels() const;
  uint8_t* GetData() const;

 private:
  int width_;
  int height_;
  int channels_;
  uint8_t* data_;
};
}  // namespace solar
