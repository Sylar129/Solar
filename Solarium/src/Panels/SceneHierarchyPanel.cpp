// Copyright (c) 2024 Sylar129

#include "SceneHierarchyPanel.h"

#include <string>

#include "Core/Scene/Components.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"           // NOLINT
#include "imgui_internal.h"  // NOLINT

namespace solar {

namespace {
void DrawVec3Control(const std::string& label, glm::vec3& values,
                     float reset_value = 0.0f, float column_width = 100.0f) {
  ImGuiIO& io = ImGui::GetIO();
  auto* bold_font = io.Fonts->Fonts[1];

  ImGui::PushID(label.c_str());

  ImGui::Columns(2);
  ImGui::SetColumnWidth(0, column_width);
  ImGui::Text("%s", label.c_str());
  ImGui::NextColumn();

  ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

  float line_height =
      GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
  ImVec2 button_size = {line_height + 3.0f, line_height};

  // X Button
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
  ImGui::PushFont(bold_font);
  if (ImGui::Button("X", button_size)) {
    values.x = reset_value;
  }
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();
  ImGui::SameLine();

  // Y Button
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.3f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
  ImGui::PushFont(bold_font);
  if (ImGui::Button("Y", button_size)) {
    values.y = reset_value;
  }
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();
  ImGui::SameLine();

  // Z Button
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
  ImGui::PushFont(bold_font);
  if (ImGui::Button("Z", button_size)) {
    values.z = reset_value;
  }
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();

  ImGui::PopStyleVar();

  ImGui::Columns(1);

  ImGui::PopID();
}

template <typename T, typename UIFunction>
void DrawComponent(const std::string& label, Entity entity,
                   UIFunction ui_function) {
  constexpr ImGuiTreeNodeFlags kTreeNodeFlags =
      ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap |
      ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
      ImGuiTreeNodeFlags_FramePadding;

  if (entity.HasComponent<T>()) {
    auto& component = entity.GetComponent<T>();
    ImVec2 content_region_available = ImGui::GetContentRegionAvail();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
    float line_height =
        GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImGui::Separator();
    // NOLINTNEXTLINE
    bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), kTreeNodeFlags,
                                  "%s", label.c_str());
    ImGui::PopStyleVar();

    ImGui::SameLine(content_region_available.x - line_height * 0.5f);
    if (ImGui::Button("+", ImVec2{line_height, line_height})) {
      ImGui::OpenPopup("ComponentSettings");
    }

    bool remove_component = false;
    if (ImGui::BeginPopup("ComponentSettings")) {
      if (ImGui::MenuItem("Remove Component")) {
        remove_component = true;
      }
      ImGui::EndPopup();
    }

    if (open) {
      ui_function(component);
      ImGui::TreePop();
    }

    if (remove_component) {
      entity.RemoveComponent<T>();
    }
  }
}
}  // namespace

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
  SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
  context_ = context;
  selection_context_ = {};
}

void SceneHierarchyPanel::OnImGuiRender() {
  // Scene Hierarchy
  ImGui::Begin("Scene Hierarchy");

  for (auto [entityID, _] :
       context_->registry_.view<TransformComponent>().each()) {
    Entity entity{entityID, context_.get()};
    DrawEntityNode(entity);
  }

  if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
    selection_context_ = {};
  }

  // Right-click on blank space
  if (ImGui::BeginPopupContextWindow(0, 1)) {
    if (ImGui::MenuItem("Create Empty Entity")) {
      context_->CreateEntity("Empty Entity");
    }
    ImGui::EndPopup();
  }

  ImGui::End();

  // Properties
  ImGui::Begin("Properties");
  if (selection_context_) {
    DrawComponents(selection_context_);
  }
  ImGui::End();
}

void SceneHierarchyPanel::SetSelectedEntity(Entity entity) {
  selection_context_ = entity;
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
  auto& tag = entity.GetComponent<TagComponent>().tag;

  ImGuiTreeNodeFlags flags =
      ((selection_context_ == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
      ImGuiTreeNodeFlags_OpenOnArrow;
  flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
  // NOLINTNEXTLINE
  bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags,
                                  "%s", tag.c_str());
  if (ImGui::IsItemClicked()) {
    selection_context_ = entity;
  }

  bool entity_deleted = false;
  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem("Delete Entity")) {
      entity_deleted = true;
    }
    ImGui::EndPopup();
  }

  if (opened) {
    ImGui::TreePop();
  }

  if (entity_deleted) {
    context_->DestoryEntity(entity);
    if (selection_context_ == entity) {
      selection_context_ = {};
    }
  }
}

void SceneHierarchyPanel::DrawComponents(Entity entity) {
  if (entity.HasComponent<TagComponent>()) {
    auto& tag = entity.GetComponent<TagComponent>().tag;

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    strcpy_s(buffer, 256, tag.c_str());
    if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
      tag = std::string(buffer);
    }
  }

  ImGui::SameLine();
  ImGui::PushItemWidth(-1);
  if (ImGui::Button("Add Component")) {
    ImGui::OpenPopup("AddComponent");
  }
  if (ImGui::BeginPopup("AddComponent")) {
    if (ImGui::MenuItem("Camera")) {
      selection_context_.AddComponent<CameraComponent>();
      ImGui::CloseCurrentPopup();
    }

    if (ImGui::MenuItem("Sprite Renderer")) {
      selection_context_.AddComponent<SpriteRendererComponent>();
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
  ImGui::PopItemWidth();

  DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
    DrawVec3Control("Translation", component.translation);

    glm::vec3 rotation = glm::degrees(component.rotation);
    DrawVec3Control("Rotation", rotation);
    component.rotation = glm::radians(rotation);

    DrawVec3Control("Scale", component.scale, 1.0f);
  });

  DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
    auto& camera = component.camera;

    ImGui::Checkbox("Primary", &component.primary);

    // TODO(sylar): simplify
    const char* projection_type_strings[] = {"Perspective", "Orthographic"};
    const char* current_projection_type_string =
        projection_type_strings[static_cast<int>(camera.GetProjectionType())];
    if (ImGui::BeginCombo("Porjection", current_projection_type_string)) {
      for (int i = 0; i < 2; ++i) {
        bool is_selected =
            current_projection_type_string == projection_type_strings[i];
        if (ImGui::Selectable(projection_type_strings[i], is_selected)) {
          current_projection_type_string = projection_type_strings[i];
          camera.SetProjectionType((SceneCamera::ProjectionType)i);
        }

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    if (camera.GetProjectionType() ==
        SceneCamera::ProjectionType::kPerspective) {
      float vertical_fov = glm::degrees(camera.GetPerspectiveVerticalFOV());
      if (ImGui::DragFloat("Vertical Fov", &vertical_fov)) {
        camera.SetPerspectiveVerticalFOV(glm::radians(vertical_fov));
      }

      float perspective_near = camera.GetPerspectiveNearClip();
      if (ImGui::DragFloat("Near", &perspective_near)) {
        camera.SetPerspectiveNearClip(perspective_near);
      }

      float perspective_far = camera.GetPerspectiveFarClip();
      if (ImGui::DragFloat("Far", &perspective_far)) {
        camera.SetPerspectiveFarClip(perspective_far);
      }
    }

    if (camera.GetProjectionType() ==
        SceneCamera::ProjectionType::kOrthographic) {
      float ortho_size = camera.GetOrthographicSize();
      if (ImGui::DragFloat("Size", &ortho_size)) {
        camera.SetOrthographicSize(ortho_size);
      }

      float ortho_near = camera.GetOrthographicNearClip();
      if (ImGui::DragFloat("Near", &ortho_near)) {
        camera.SetOrthographicNearClip(ortho_near);
      }

      float ortho_far = camera.GetOrthographicFarClip();
      if (ImGui::DragFloat("Far", &ortho_far)) {
        camera.SetOrthographicFarClip(ortho_far);
      }

      ImGui::Checkbox("Fixed Aspect Ratio", &component.fixed_aspect_ratio);
    }
  });

  DrawComponent<SpriteRendererComponent>(
      "Sprite Renderer", entity, [](auto& component) {
        ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
      });
}

}  // namespace solar
