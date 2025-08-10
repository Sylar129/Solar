project(vendor)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/Vendor)

add_library(${PROJECT_NAME} INTERFACE)

# spdlog
FetchContent_Declare(
    spdlog
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.15.3.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(spdlog)

# glm
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
target_compile_definitions(${PROJECT_NAME} INTERFACE GLM_ENABLE_EXPERIMENTAL)
FetchContent_MakeAvailable(glm)

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
target_include_directories(${PROJECT_NAME} INTERFACE ${entt_SOURCE_DIR}/src)

# stb
FetchContent_Declare(
    stb
    URL https://github.com/nothings/stb/archive/refs/heads/master.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(stb)
target_include_directories(${PROJECT_NAME} INTERFACE ${stb_SOURCE_DIR})

# yaml-cpp
FetchContent_Declare(
    yaml-cpp
    URL https://github.com/jbeder/yaml-cpp/archive/refs/heads/master.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(yaml-cpp)

# Imgui
include_directories(INTERFACE imgui/)

set(ImGuiSrc
    imgui/imconfig.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    # backends
    imgui/backends/imgui_impl_glfw.h
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_opengl3.h
    imgui/backends/imgui_impl_opengl3_loader.h)
add_library(ImGui STATIC ${ImGuiSrc})

# ImGuizmo
include_directories( INTERFACE ImGuizmo/)

set(ImGuizmoSrc
    ImGuizmo/ImGuizmo.cpp
    ImGuizmo/ImGuizmo.h)
add_library(ImGuizmo STATIC ${ImGuizmoSrc})

# glad
include_directories(INTERFACE glad/include/)

set(gladSrc
    glad/src/glad.c
    glad/include/glad/glad.h
    glad/include/KHR/khrplatform.h)
add_library(glad STATIC ${gladSrc})


target_link_libraries(${PROJECT_NAME} INTERFACE glfw ImGui ImGuizmo glad yaml-cpp::yaml-cpp spdlog::spdlog glm::glm)
target_include_directories(${PROJECT_NAME} INTERFACE imgui/ ImGuizmo/ glad/include/)
