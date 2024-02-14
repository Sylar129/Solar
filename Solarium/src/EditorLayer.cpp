// Copyright (c) 2024 Sylar129

#include "EditorLayer.h"

#include <string>

// clang-format off
#include "imgui.h" // NOLINT
#include "ImGuizmo.h"
// clang-format on

#include "Core/Debug/Instrumentor.h"
#include "Core/Math/Math.h"
#include "Core/Scene/SceneSerializer.h"
#include "Core/Utils/PlatformUtils.h"
#include "glm/gtc/type_ptr.hpp"

namespace solar {

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), camera_controller_(1280.0f / 720.0f) {}

void EditorLayer::OnAttach() {
  SOLAR_PROFILE_FUNCTION();

  FramebufferSpecification fb_spec;
  fb_spec.attachments = {FramebufferTextureFormat::kRGBA8,
                         FramebufferTextureFormat::kRED_INTEGER,
                         FramebufferTextureFormat::kDepth};
  fb_spec.width = 1280;
  fb_spec.height = 720;
  framebuffer_ = Framebuffer::Create(fb_spec);

  active_scene_ = CreateRef<Scene>();

  editor_camera_ = EditorCamera(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

#if 0
  // Entity
  auto square = active_scene_->CreateEntity("Green Square");
  square.AddComponent<SpriteRendererComponent>(
      glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

  auto red_square = active_scene_->CreateEntity("Red Square");
  red_square.AddComponent<SpriteRendererComponent>(
      glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

  square_entity_ = square;

  camera_entity_ = active_scene_->CreateEntity("Camera A");
  camera_entity_.AddComponent<CameraComponent>();

  second_camera_ = active_scene_->CreateEntity("Camera B");
  auto& cc = second_camera_.AddComponent<CameraComponent>();
  cc.primary = false;

  class CameraController : public ScriptableEntity {
   public:
    void OnCreate() {}

    void OnDestory() {}

    void OnUpdate(TimeStep ts) {
      auto& translation = GetComponent<TransformComponent>().translation;
      float speed = 5.0f;

      if (Input::IsKeyPressed(KeyCode::kA)) {
        translation.x -= speed * ts;
      }
      if (Input::IsKeyPressed(KeyCode::kD)) {
        translation.x += speed * ts;
      }
      if (Input::IsKeyPressed(KeyCode::kW)) {
        translation.y += speed * ts;
      }
      if (Input::IsKeyPressed(KeyCode::kS)) {
        translation.y -= speed * ts;
      }
    }
  };

  second_camera_.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
  scene_hierarchy_panel_.SetContext(active_scene_);
}

void EditorLayer::OnDetech() { SOLAR_PROFILE_FUNCTION(); }

void EditorLayer::OnUpdate(TimeStep& ts) {
  SOLAR_PROFILE_FUNCTION();

  // Resize
  if (FramebufferSpecification spec = framebuffer_->GetSpecification();
      viewport_size_.x > 0.0f &&
      viewport_size_.y > 0.0f  // zero sized framebuffer is invalid
      && (spec.width != viewport_size_.x || spec.height != viewport_size_.y)) {
    framebuffer_->Resize((uint32_t)viewport_size_.x,
                         (uint32_t)viewport_size_.y);
    camera_controller_.OnResize(viewport_size_.x, viewport_size_.y);
    editor_camera_.SetViewportSize(viewport_size_.x, viewport_size_.y);
    active_scene_->OnViewportResize((uint32_t)viewport_size_.x,
                                    (uint32_t)viewport_size_.y);
  }

  if (viewport_focused_) {
    camera_controller_.OnUpdate(ts);
  }
  editor_camera_.OnUpdate(ts);

  /// <summary>
  /// Renderer
  /// </summary>
  Renderer2D::ResetStats();
  framebuffer_->Bind();
  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
  RenderCommand::Clear();

  // Clear entity ID attachment to -1
  framebuffer_->ClearAttachment(1, -1);

  // Update scene
  active_scene_->OnUpdateEditor(ts, editor_camera_);

  auto [mx, my] = ImGui::GetMousePos();
  mx -= viewport_bounds_[0].x;
  my -= viewport_bounds_[0].y;
  auto viewport_size = viewport_bounds_[1] - viewport_bounds_[0];
  my = viewport_size.y - my;

  int mouse_x = static_cast<int>(mx);
  int mouse_y = static_cast<int>(my);

  if (mouse_x >= 0 && mouse_y >= 0 &&
      mouse_x < static_cast<int>(viewport_size.x) &&
      mouse_y < static_cast<int>(viewport_size.y)) {
    int pixel_data = framebuffer_->ReadPixel(1, mouse_x, mouse_y);
    if (-1 == pixel_data) {
      hovered_entity_ = {};
    } else {
      hovered_entity_ = {(entt::entity)pixel_data, active_scene_.get()};
    }
  }

  framebuffer_->Unbind();
}

void EditorLayer::OnImGuiRender() {
  SOLAR_PROFILE_FUNCTION();

  static bool dockspace_open = true;
  static bool opt_fullscreen = true;
  static bool opt_padding = false;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
  // not dockable into, because it would be confusing to have two docking
  // targets within each others.
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (opt_fullscreen) {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  } else {
    dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
  }

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render
  // our background and handle the pass-thru hole, so we ask Begin() to not
  // render a background.
  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
    window_flags |= ImGuiWindowFlags_NoBackground;
  }

  // Important: note that we proceed even if Begin() returns false (aka window
  // is collapsed). This is because we want to keep our DockSpace() active. If a
  // DockSpace() is inactive, all active windows docked into it will lose their
  // parent and become undocked. We cannot preserve the docking relationship
  // between an active window and an inactive docking, otherwise any change of
  // dockspace/settings would lead to windows being stuck in limbo and never
  // being visible.
  if (!opt_padding) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  }
  ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
  if (!opt_padding) {
    ImGui::PopStyleVar();
  }

  if (opt_fullscreen) {
    ImGui::PopStyleVar(2);
  }

  // Submit the DockSpace
  ImGuiIO& io = ImGui::GetIO();
  ImGuiStyle& style = ImGui::GetStyle();
  float min_win_size_x = style.WindowMinSize.x;
  style.WindowMinSize.x = 370.f;
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  style.WindowMinSize.x = min_win_size_x;

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      // Disabling fullscreen would allow the window to be moved to the front of
      // other windows, which we can't undo at the moment without finer window
      // depth/z control.
      ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
      ImGui::MenuItem("Padding", NULL, &opt_padding);
      ImGui::Separator();

      if (ImGui::MenuItem("New", "Ctrl+N")) {
        NewScene();
      }

      if (ImGui::MenuItem("Open... ", "Ctrl+O")) {
        OpenScene();
      }

      if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
        SaveSceneAs();
      }
      ImGui::Separator();

      if (ImGui::MenuItem("Exit")) {
        Application::Get().Close();
      }
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  // Panels
  scene_hierarchy_panel_.OnImGuiRender();

  // Stats
  ImGui::Begin("Stats");

  std::string name = "None";
  if (hovered_entity_) {
    name = hovered_entity_.GetComponent<TagComponent>().tag;
  }
  ImGui::Text("Hovered Entity: %s", name.c_str());

  auto stats = Renderer2D::GetStats();
  ImGui::Text("Renderer2D stats:");
  ImGui::Text("Draw calls: %d", stats.draw_calls);
  ImGui::Text("Quads: %d", stats.quad_count);
  ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
  ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
  ImGui::End();

  // Viewport
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::Begin("Viewport");
  auto viewport_offset = ImGui::GetCursorPos();  // Includes tab bar

  viewport_focused_ = ImGui::IsWindowFocused();
  viewport_hovered_ = ImGui::IsWindowHovered();
  Application::Get().GetImGuiLayer()->SetBlockEvents(!viewport_focused_ &&
                                                     !viewport_hovered_);

  ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
  viewport_size_ = {viewport_panel_size.x, viewport_panel_size.y};
  uint32_t texture_id = framebuffer_->GetColorAttachmentRendererID(0);
  ImGui::Image((void*)texture_id, ImVec2(viewport_size_.x, viewport_size_.y),
               ImVec2{0, 1}, ImVec2{1, 0});

  auto window_size = ImGui::GetWindowSize();
  ImVec2 min_bound = ImGui::GetWindowPos();
  min_bound.x += viewport_offset.x;
  min_bound.y += viewport_offset.y;

  ImVec2 max_bound = {min_bound.x + window_size.x, min_bound.y + window_size.y};
  viewport_bounds_[0] = {min_bound.x, min_bound.y};
  viewport_bounds_[1] = {max_bound.x, max_bound.y};

  // Gizmos
  Entity selected_entity = scene_hierarchy_panel_.GetSelectedEntity();
  if (selected_entity && gizmo_type_ != -1) {
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    float window_width = ImGui::GetWindowWidth();
    float window_height = ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                      window_width, window_height);

    // Camera
    // Runtime camera from entity
    // auto cameraEntity = active_scene_->GetPrimaryCameraEntity();
    // const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
    // const glm::mat4 cameraProjection = camera.GetProjection();
    // glm::mat4 cameraView =
    // glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTranform());

    // Editor camera
    glm::mat4 camera_projection = editor_camera_.GetProjection();
    glm::mat4 camera_view = editor_camera_.GetViewMatrix();

    // Entity transform
    auto& tc = selected_entity.GetComponent<TransformComponent>();
    glm::mat4 transform = tc.GetTranform();

    // Snapping
    bool snap = Input::IsKeyPressed(KeyCode::kLeftControl);
    // Snap to 0.5m for translation/scale
    float snap_value = 0.5f;
    // Snap to 45 degrees for rotation
    if (ImGuizmo::OPERATION::ROTATE == gizmo_type_) {
      snap_value = 45.0f;
    }

    float snap_values[3] = {snap_value, snap_value, snap_value};

    ImGuizmo::Manipulate(
        glm::value_ptr(camera_view), glm::value_ptr(camera_projection),
        (ImGuizmo::OPERATION)gizmo_type_, ImGuizmo::LOCAL,
        glm::value_ptr(transform), nullptr, snap ? snap_values : nullptr);

    if (ImGuizmo::IsUsing()) {
      glm::vec3 translation;
      glm::vec3 rotation;
      glm::vec3 scale;
      Math::DecomposeTranform(transform, translation, rotation, scale);

      glm::vec3 delta_rotation = rotation - tc.rotation;
      tc.translation = translation;
      tc.rotation += delta_rotation;
      tc.scale = scale;
    }
  }

  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::End();
}

void EditorLayer::OnEvent(Event& event) {
  // SOLAR_TRACE("ExampleLayer: {0}", event);
  camera_controller_.OnEvent(event);
  editor_camera_.OnEvent(event);

  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<KeyPressdEvent>(
      SOLAR_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
  dispatcher.Dispatch<MouseButtonPressedEvent>(
      SOLAR_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
}

bool EditorLayer::OnKeyPressed(KeyPressdEvent& e) {
  // Shortcuts
  if (e.GetRepeatCount() > 0) {
    return false;
  }

  bool control = Input::IsKeyPressed(KeyCode::kLeftControl) ||
                 Input::IsKeyPressed(KeyCode::kRightControl);
  bool shift = Input::IsKeyPressed(KeyCode::kLeftShift) ||
               Input::IsKeyPressed(KeyCode::kRightShift);
  switch (e.GetKeyCode()) {
    case KeyCode::kN:
      if (control) {
        NewScene();
      }
      break;

    case KeyCode::kO:
      if (control) {
        OpenScene();
      }
      break;
    case KeyCode::kS:
      if (control && shift) {
        SaveSceneAs();
      }
      break;

      // Gizmos
    case KeyCode::kQ:
      gizmo_type_ = -1;
      break;
    case KeyCode::kW:
      gizmo_type_ = ImGuizmo::OPERATION::TRANSLATE;
      break;
    case KeyCode::kE:
      gizmo_type_ = ImGuizmo::OPERATION::ROTATE;
      break;
    case KeyCode::kR:
      gizmo_type_ = ImGuizmo::OPERATION::SCALE;
      break;
    default:
      break;
  }
  return false;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
  // TODO(sylar): call function to detect state
  if (e.GetMouseButton() == MouseCode::kButtonLeft &&
      !Input::IsKeyPressed(KeyCode::kLeftAlt)) {
    if (viewport_hovered_ && !ImGuizmo::IsOver()) {
      scene_hierarchy_panel_.SetSelectedEntity(hovered_entity_);
    }
  }
  return false;
}

void EditorLayer::NewScene() {
  active_scene_ = CreateRef<Scene>();
  active_scene_->OnViewportResize((uint32_t)viewport_size_.x,
                                  (uint32_t)viewport_size_.y);
  scene_hierarchy_panel_.SetContext(active_scene_);
}

void EditorLayer::OpenScene() {
  std::string filepath =
      FileDialogs::OpenFile("Solar Scene(*.solar)\0*.solar\0");
  if (!filepath.empty()) {
    active_scene_ = CreateRef<Scene>();
    active_scene_->OnViewportResize((uint32_t)viewport_size_.x,
                                    (uint32_t)viewport_size_.y);
    scene_hierarchy_panel_.SetContext(active_scene_);

    SceneSerializer serializer(active_scene_);
    serializer.Deserialize(filepath);
  }
}

void EditorLayer::SaveSceneAs() {
  std::string filepath =
      FileDialogs::SaveFile("Solar Scene(*.solar)\0*.solar\0");
  if (!filepath.empty()) {
    SceneSerializer serializer(active_scene_);
    serializer.Serialize(filepath);
  }
}

}  // namespace solar
