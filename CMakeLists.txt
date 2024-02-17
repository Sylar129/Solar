cmake_minimum_required(VERSION 3.20)

project(Solarium)

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

include_directories(
  Solar/
  Solarium/
  Vendor/imgui/
  Vendor/ImGuizmo/
  Vendor/glm/
  Vendor/spdlog/include/
  Vendor/entt/src/)

# Solarium
add_executable(
  Solarium
  Solarium/src/panels/content_browser_panel.cpp
  Solarium/src/panels/content_browser_panel.h
  Solarium/src/panels/scene_hierarchy_panel.cpp
  Solarium/src/panels/scene_hierarchy_panel.h
  Solarium/src/editor_layer.cpp
  Solarium/src/editor_layer.h
  Solarium/src/solarium_app.cpp)

# Solar
add_subdirectory(Solar/)

# Link
target_link_libraries(Solarium PUBLIC Solar)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
  # add_compile_definitions(SOLAR_PLATFORM_WINDOWS)
  set_target_properties(Solarium PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
                                            "${PROJECT_SOURCE_DIR}")
endif()
