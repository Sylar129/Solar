// Copyright (c) 2024 Sylar129

#pragma once

#include <filesystem>

#include "core/renderer/texture.h"
#include "core/utils/misc.h"

namespace solar {
class ContentBrowserPanel {
 public:
  ContentBrowserPanel();

  void OnImGuiRender();

 private:
  std::filesystem::path current_directory_;
  Ref<Texture2D> directory_icon_;
  Ref<Texture2D> file_icon_;
};
}  // namespace solar
