project(Windows)

file(GLOB SOURCES "./*cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(Solar::Windows ALIAS ${PROJECT_NAME})
