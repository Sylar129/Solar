// Copyright (c) 2024 Sylar129

#include "src/panels/content_browser_panel.h"

#include <algorithm>
#include <filesystem>
#include <string>
#include <string_view>

#include "imgui.h"  // NOLINT

namespace solar {

constexpr std::string_view kAssertDirectory = "assets";

ContentBrowserPanel::ContentBrowserPanel()
    : current_directory_(kAssertDirectory) {
  directory_icon_ = Texture2D::Create("resources/icons/open-folder.png");
  file_icon_ = Texture2D::Create("resources/icons/files.png");
}

void ContentBrowserPanel::OnImGuiRender() {
  ImGui::Begin("Content Browser");

  if (current_directory_.string() != kAssertDirectory) {
    if (ImGui::Button("<-")) {
      current_directory_ = current_directory_.parent_path();
    }
  }

  // temporary
  constexpr float kPadding = 16;
  constexpr float kThumbnailSize = 128;
  constexpr float kCellSize = kPadding + kThumbnailSize;

  float browser_width = ImGui::GetContentRegionAvail().x;
  int count = std::max(1, static_cast<int>(browser_width / kCellSize));
  ImGui::Columns(count, 0, false);

  for (const auto& p :
       std::filesystem::directory_iterator(current_directory_)) {
    const auto& filename = p.path().filename().string();

    Ref<Texture2D> icon = p.is_directory() ? directory_icon_ : file_icon_;

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::ImageButton((ImTextureID)icon->GetRendererID(),
                       {kThumbnailSize, kThumbnailSize}, {0, 1}, {1, 0});
    ImGui::PopStyleColor();

    if (ImGui::IsItemHovered() &&
        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
      if (p.is_directory()) {
        current_directory_ /= p.path().filename();
      }
    }
    ImGui::TextWrapped("%s", filename.c_str());
    ImGui::NextColumn();
  }

  ImGui::Columns(1);
  ImGui::End();
}
}  // namespace solar
