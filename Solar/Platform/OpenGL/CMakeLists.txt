project(OpenGL)

file(GLOB SOURCES "./*cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(Solar::OpenGL ALIAS ${PROJECT_NAME})
