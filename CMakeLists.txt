cmake_minimum_required(VERSION 3.20)

project(Solarium)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

include_directories(
  Solar/
  Vendor/imgui/
  Vendor/ImGuizmo/
  Vendor/glm/
  Vendor/spdlog/include/
  Vendor/entt/src/
)

# Solarium
add_executable(Solarium
  Solarium/src/EditorLayer.cpp
  Solarium/src/EditorLayer.h
  Solarium/src/SolariumApp.cpp
  Solarium/src/Panels/SceneHierarchyPanel.cpp
  Solarium/src/Panels/SceneHierarchyPanel.h
)

# Solar
add_subdirectory(Solar/)

# Link
target_link_libraries(Solarium PUBLIC Solar)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
  add_compile_definitions(SOLAR_PLATFORM_WINDOWS)
endif()
