cmake_minimum_required(VERSION 3.20)

project(Solarium)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/Vendor)

# spdlog
FetchContent_Declare(
    spdlog
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.15.3.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(spdlog)
# include_directories(AFTER ${spdlog_SOURCE_DIR}/include)

# glm
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
FetchContent_MakeAvailable(glm)
# include_directories(AFTER ${glm_SOURCE_DIR}/include)

# glfw
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/3.4.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(glfw)
include_directories(AFTER ${glfw_SOURCE_DIR}/include)

# entt
FetchContent_Declare(
    entt
    URL https://github.com/skypjack/entt/archive/refs/tags/v3.15.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(entt)
include_directories(AFTER ${entt_SOURCE_DIR}/src)

# stb
FetchContent_Declare(
    stb
    URL https://github.com/nothings/stb/archive/refs/heads/master.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(stb)
include_directories(AFTER ${stb_SOURCE_DIR})

# yaml-cpp
FetchContent_Declare(
    yaml-cpp
    URL https://github.com/jbeder/yaml-cpp/archive/refs/heads/master.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(yaml-cpp)
# include_directories(AFTER ${yaml-cpp_SOURCE_DIR})

include_directories(PUBLIC
  ./
  ../Vendor/imgui/
  ../Vendor/glad/include/
  ../Vendor/ImGuizmo/)

# Solar
set(SolarSrc
    core/base/application.cpp
    core/base/application.h
    core/base/entrypoint.h
    core/base/input.h
    core/base/key_codes.h
    core/base/layer_stack.cpp
    core/base/layer_stack.h
    core/base/layer.cpp
    core/base/layer.h
    core/base/log.cpp
    core/base/log.h
    core/base/mouse_codes.h
    core/base/timestep.h
    core/base/window.h
    core/debug/assert.h
    core/debug/instrumentor.h
    core/events/application_event.h
    core/events/event.h
    core/events/key_event.h
    core/events/mouse_event.h
    core/imgui/imgui_layer.cpp
    core/imgui/imgui_layer.h
    core/math/math.cpp
    core/math/math.h
    core/math/size.h
    core/renderer/buffer.cpp
    core/renderer/buffer.h
    core/renderer/camera.h
    core/renderer/editor_camera.cpp
    core/renderer/editor_camera.h
    core/renderer/framebuffer.cpp
    core/renderer/framebuffer.h
    core/renderer/graphics_context.h
    core/renderer/orthographic_camera_controller.cpp
    core/renderer/orthographic_camera_controller.h
    core/renderer/orthographic_camera.cpp
    core/renderer/orthographic_camera.h
    core/renderer/render_command.cpp
    core/renderer/render_command.h
    core/renderer/renderer_api.cpp
    core/renderer/renderer_api.h
    core/renderer/renderer.cpp
    core/renderer/renderer.h
    core/renderer/renderer2d_data.h
    core/renderer/renderer2d.cpp
    core/renderer/renderer2d.h
    core/renderer/shader_data_type.h
    core/renderer/shader.cpp
    core/renderer/shader.h
    core/renderer/subtexture2d.cpp
    core/renderer/subtexture2d.h
    core/renderer/texture.cpp
    core/renderer/texture.h
    core/renderer/vertex_array.cpp
    core/renderer/vertex_array.h
    core/scene/components.h
    core/scene/entity.cpp
    core/scene/entity.h
    core/scene/scene_camera.cpp
    core/scene/scene_camera.h
    core/scene/scene_serializer.cpp
    core/scene/scene_serializer.h
    core/scene/scene.cpp
    core/scene/scene.h
    core/scene/scriptable_entity.h
    core/utils/image.cpp
    core/utils/image.h
    core/utils/misc.h
    core/utils/platform_utils.h
    platform/opengl/buffer.cpp
    platform/opengl/buffer.h
    platform/opengl/context.cpp
    platform/opengl/context.h
    platform/opengl/framebuffer.cpp
    platform/opengl/framebuffer.h
    platform/opengl/renderer_api.cpp
    platform/opengl/renderer_api.h
    platform/opengl/shader.cpp
    platform/opengl/shader.h
    platform/opengl/texture.cpp
    platform/opengl/texture.h
    platform/opengl/vertex_array.cpp
    platform/opengl/vertex_array.h)
add_library(Solar STATIC ${SolarSrc})
target_precompile_headers(Solar PRIVATE solpch.h)

# Imgui
set(ImGuiSrc
    ../Vendor/imgui/imconfig.h
    ../Vendor/imgui/imgui_demo.cpp
    ../Vendor/imgui/imgui_draw.cpp
    ../Vendor/imgui/imgui_internal.h
    ../Vendor/imgui/imgui_tables.cpp
    ../Vendor/imgui/imgui_widgets.cpp
    ../Vendor/imgui/imgui.cpp
    ../Vendor/imgui/imgui.h
    ../Vendor/imgui/imstb_rectpack.h
    ../Vendor/imgui/imstb_textedit.h
    ../Vendor/imgui/imstb_truetype.h
    # backends
    ../Vendor/imgui/backends/imgui_impl_glfw.h
    ../Vendor/imgui/backends/imgui_impl_glfw.cpp
    ../Vendor/imgui/backends/imgui_impl_opengl3.cpp
    ../Vendor/imgui/backends/imgui_impl_opengl3.h
    ../Vendor/imgui/backends/imgui_impl_opengl3_loader.h)
add_library(ImGui STATIC ${ImGuiSrc})

# ImGuizmo
set(ImGuizmoSrc ../Vendor/ImGuizmo/ImGuizmo.cpp ../Vendor/ImGuizmo/ImGuizmo.h)
add_library(ImGuizmo STATIC ${ImGuizmoSrc})

# glad
set(gladSrc ../Vendor/glad/src/glad.c ../Vendor/glad/include/glad/glad.h
            ../Vendor/glad/include/KHR/khrplatform.h)
add_library(glad STATIC ${gladSrc})

target_link_libraries(Solar PUBLIC glfw ImGui ImGuizmo glad yaml-cpp::yaml-cpp spdlog::spdlog glm::glm)
