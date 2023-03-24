project(Solar)


include_directories(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE SRC_FILES "./*.cpp")
file(GLOB_RECURSE HEADER_FILES "./*.h")

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEADER_FILES})
add_library(Solar::Solar ALIAS ${PROJECT_NAME})

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
    yaml-cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

target_precompile_headers(${PROJECT_NAME} PUBLIC solpch.h)
