// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

namespace solar {

class FileDialogs {
 public:
  // These return empty strings if cancelled
  static std::string OpenFile(const char* filter);
  static std::string SaveFile(const char* filter);
};

}  // namespace solar
