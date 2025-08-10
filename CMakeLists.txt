cmake_minimum_required(VERSION 3.20)

project(Solar)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# temporary
add_compile_definitions(SOLAR_PLATFORM_WINDOWS)
add_compile_definitions(SOLAR_DEBUG)

include_directories(PUBLIC
  Solar/)

# Solar
set(SolarSrc
    Solar/core/base/application.cpp
    Solar/core/base/application.h
    Solar/core/base/entrypoint.h
    Solar/core/base/input.h
    Solar/core/base/key_codes.h
    Solar/core/base/layer_stack.cpp
    Solar/core/base/layer_stack.h
    Solar/core/base/layer.cpp
    Solar/core/base/layer.h
    Solar/core/base/log.cpp
    Solar/core/base/log.h
    Solar/core/base/mouse_codes.h
    Solar/core/base/timestep.h
    Solar/core/base/window.h
    Solar/core/debug/assert.h
    Solar/core/debug/instrumentor.h
    Solar/core/events/application_event.h
    Solar/core/events/event.h
    Solar/core/events/key_event.h
    Solar/core/events/mouse_event.h
    Solar/core/imgui/imgui_layer.cpp
    Solar/core/imgui/imgui_layer.h
    Solar/core/math/math.cpp
    Solar/core/math/math.h
    Solar/core/math/size.h
    Solar/core/renderer/buffer.cpp
    Solar/core/renderer/buffer.h
    Solar/core/renderer/camera.h
    Solar/core/renderer/editor_camera.cpp
    Solar/core/renderer/editor_camera.h
    Solar/core/renderer/framebuffer.cpp
    Solar/core/renderer/framebuffer.h
    Solar/core/renderer/graphics_context.h
    Solar/core/renderer/orthographic_camera_controller.cpp
    Solar/core/renderer/orthographic_camera_controller.h
    Solar/core/renderer/orthographic_camera.cpp
    Solar/core/renderer/orthographic_camera.h
    Solar/core/renderer/render_command.cpp
    Solar/core/renderer/render_command.h
    Solar/core/renderer/renderer_api.cpp
    Solar/core/renderer/renderer_api.h
    Solar/core/renderer/renderer.cpp
    Solar/core/renderer/renderer.h
    Solar/core/renderer/renderer2d_data.h
    Solar/core/renderer/renderer2d.cpp
    Solar/core/renderer/renderer2d.h
    Solar/core/renderer/shader_data_type.h
    Solar/core/renderer/shader.cpp
    Solar/core/renderer/shader.h
    Solar/core/renderer/subtexture2d.cpp
    Solar/core/renderer/subtexture2d.h
    Solar/core/renderer/texture.cpp
    Solar/core/renderer/texture.h
    Solar/core/renderer/vertex_array.cpp
    Solar/core/renderer/vertex_array.h
    Solar/core/scene/components.h
    Solar/core/scene/entity.cpp
    Solar/core/scene/entity.h
    Solar/core/scene/scene_camera.cpp
    Solar/core/scene/scene_camera.h
    Solar/core/scene/scene_serializer.cpp
    Solar/core/scene/scene_serializer.h
    Solar/core/scene/scene.cpp
    Solar/core/scene/scene.h
    Solar/core/scene/scriptable_entity.h
    Solar/core/utils/image.cpp
    Solar/core/utils/image.h
    Solar/core/utils/misc.h
    Solar/core/utils/platform_utils.h
    Solar/platform/opengl/buffer.cpp
    Solar/platform/opengl/buffer.h
    Solar/platform/opengl/context.cpp
    Solar/platform/opengl/context.h
    Solar/platform/opengl/framebuffer.cpp
    Solar/platform/opengl/framebuffer.h
    Solar/platform/opengl/renderer_api.cpp
    Solar/platform/opengl/renderer_api.h
    Solar/platform/opengl/shader.cpp
    Solar/platform/opengl/shader.h
    Solar/platform/opengl/texture.cpp
    Solar/platform/opengl/texture.h
    Solar/platform/opengl/vertex_array.cpp
    Solar/platform/opengl/vertex_array.h
    Solar/platform/windows/input.cpp
    Solar/platform/windows/platform_utils.cpp
    Solar/platform/windows/window.cpp)





add_executable(
  Solar
  Solar/editor/panels/content_browser_panel.cpp
  Solar/editor/panels/content_browser_panel.h
  Solar/editor/panels/scene_hierarchy_panel.cpp
  Solar/editor/panels/scene_hierarchy_panel.h
  Solar/editor/editor_layer.cpp
  Solar/editor/editor_layer.h
  Solar/editor/solarium_app.cpp
  ${SolarSrc})

target_precompile_headers(Solar PRIVATE Solar/solpch.h)

add_subdirectory(vendor/)

target_link_libraries(Solar PUBLIC vendor)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
  # add_compile_definitions(SOLAR_PLATFORM_WINDOWS)
  set_target_properties(Solar PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
                                            "${PROJECT_SOURCE_DIR}")
endif()
