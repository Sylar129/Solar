project(Solar)

add_library(${PROJECT_NAME} STATIC solpch.cpp)
add_library(Solar::Solar ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}
)

add_subdirectory(Core)
add_subdirectory(Platform)

# 3rd libraries
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    glm::glm
PRIVATE
    EnTT::EnTT
    imgui::imgui
    imguizmo::imguizmo
    spdlog::spdlog
    spdlog::spdlog_header_only
    yaml-cpp
###############
    Solar::OpenGL
    Solar::Windows)

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

target_precompile_headers(${PROJECT_NAME} PRIVATE solpch.h)
