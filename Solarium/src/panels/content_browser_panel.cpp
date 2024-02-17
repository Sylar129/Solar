// Copyright (c) 2024 Sylar129

#include "src/panels/content_browser_panel.h"

#include <filesystem>
#include <string>
#include <string_view>

#include "imgui.h"  // NOLINT

namespace solar {

constexpr std::string_view kAssertDirectory = "assets";

ContentBrowserPanel::ContentBrowserPanel()
    : current_directory_(kAssertDirectory) {}

void ContentBrowserPanel::OnImGuiRender() {
  ImGui::Begin("Content Browser");

  if (current_directory_.string() != kAssertDirectory) {
    if (ImGui::Button("<-")) {
      current_directory_ = current_directory_.parent_path();
    }
  }

  for (const auto& p :
       std::filesystem::directory_iterator(current_directory_)) {
    const auto& filename = p.path().filename().string();
    if (p.is_directory()) {
      if (ImGui::Button(filename.c_str())) {
        current_directory_ /= p.path().filename();
      }
    } else {
      if (ImGui::Button(filename.c_str())) {
        // TODO(sylar): load file
      }
    }
  }

  ImGui::End();
}
}  // namespace solar
