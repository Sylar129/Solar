project(Solarium)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Solar::Solar)

find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    glm::glm)

include_directories(${PROJECT_SOURCE_DIR}/../Solar)
