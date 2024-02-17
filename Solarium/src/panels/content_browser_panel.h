// Copyright (c) 2024 Sylar129

#pragma once

#include <filesystem>
namespace solar {
class ContentBrowserPanel {
 public:
  ContentBrowserPanel();

  void OnImGuiRender();

 private:
  std::filesystem::path current_directory_;
};
}  // namespace solar
