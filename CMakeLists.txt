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
  Vendor/glfw/include/
  Vendor/glad/include/
  Vendor/spdlog/include/
  Vendor/glm/
  Vendor/entt/src/
  Vendor/ImGuizmo/
  Vendor/yaml-cpp/include
  Vendor/stb/
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
set(SolarSrc
  Solar/Solar.h
  Solar/Core/Base/Application.cpp
  Solar/Core/Base/Application.h
  Solar/Core/Base/EntryPoint.h
  Solar/Core/Base/Input.h
  Solar/Core/Base/KeyCodes.h
  Solar/Core/Base/Layer.cpp
  Solar/Core/Base/Layer.h
  Solar/Core/Base/LayerStack.cpp
  Solar/Core/Base/LayerStack.h
  Solar/Core/Base/Log.cpp
  Solar/Core/Base/Log.h
  Solar/Core/Base/MouseCodes.h
  Solar/Core/Base/Sundry.h
  Solar/Core/Base/Timestep.h
  Solar/Core/Base/Window.h
  Solar/Core/Debug/Instrumentor.h
  Solar/Core/Events/ApplicationEvent.h
  Solar/Core/Events/Event.h
  Solar/Core/Events/KeyEvent.h
  Solar/Core/Events/MouseEvent.h
  Solar/Core/ImGui/ImGuiLayer.cpp
  Solar/Core/ImGui/ImGuiLayer.h
  Solar/Core/Math/Math.cpp
  Solar/Core/Math/Math.h
  Solar/Core/Renderer/Buffer.cpp
  Solar/Core/Renderer/Buffer.h
  Solar/Core/Renderer/Camera.h
  Solar/Core/Renderer/EditorCamera.cpp
  Solar/Core/Renderer/EditorCamera.h
  Solar/Core/Renderer/Framebuffer.cpp
  Solar/Core/Renderer/Framebuffer.h
  Solar/Core/Renderer/GraphicsContext.h
  Solar/Core/Renderer/OrthographicCamera.cpp
  Solar/Core/Renderer/OrthographicCamera.h
  Solar/Core/Renderer/OrthographicCameraController.cpp
  Solar/Core/Renderer/OrthographicCameraController.h
  Solar/Core/Renderer/RenderCommand.cpp
  Solar/Core/Renderer/RenderCommand.h
  Solar/Core/Renderer/Renderer.cpp
  Solar/Core/Renderer/Renderer.h
  Solar/Core/Renderer/Renderer2D.cpp
  Solar/Core/Renderer/Renderer2D.h
  Solar/Core/Renderer/RendererAPI.cpp
  Solar/Core/Renderer/RendererAPI.h
  Solar/Core/Renderer/Shader.cpp
  Solar/Core/Renderer/Shader.h
  Solar/Core/Renderer/SubTexture2D.cpp
  Solar/Core/Renderer/SubTexture2D.h
  Solar/Core/Renderer/Texture.cpp
  Solar/Core/Renderer/Texture.h
  Solar/Core/Renderer/VertexArray.cpp
  Solar/Core/Renderer/VertexArray.h
  Solar/Core/Scene/Components.h
  Solar/Core/Scene/Entity.cpp
  Solar/Core/Scene/Entity.h
  Solar/Core/Scene/Scene.cpp
  Solar/Core/Scene/Scene.h
  Solar/Core/Scene/SceneCamera.cpp
  Solar/Core/Scene/SceneCamera.h
  Solar/Core/Scene/SceneSerializer.cpp
  Solar/Core/Scene/SceneSerializer.h
  Solar/Core/Scene/ScriptableEntity.h
  Solar/Core/Utils/PlatformUtils.h

  Solar/Platform/OpenGL/OpenGLBuffer.cpp
  Solar/Platform/OpenGL/OpenGLBuffer.h
  Solar/Platform/OpenGL/OpenGLContext.cpp
  Solar/Platform/OpenGL/OpenGLContext.h
  Solar/Platform/OpenGL/OpenGLFramebuffer.cpp
  Solar/Platform/OpenGL/OpenGLFramebuffer.h
  Solar/Platform/OpenGL/OpenGLRendererAPI.cpp
  Solar/Platform/OpenGL/OpenGLRendererAPI.h
  Solar/Platform/OpenGL/OpenGLShader.cpp
  Solar/Platform/OpenGL/OpenGLShader.h
  Solar/Platform/OpenGL/OpenGLTexture.cpp
  Solar/Platform/OpenGL/OpenGLTexture.h
  Solar/Platform/OpenGL/OpenGLVertexArray.cpp
  Solar/Platform/OpenGL/OpenGLVertexArray.h

  Solar/Platform/Windows/WindowsInput.cpp
  Solar/Platform/Windows/WindowsPlatformUtils.cpp
  Solar/Platform/Windows/WindowsWindow.cpp
  Solar/Platform/Windows/WindowsWindow.h
)
add_library(Solar STATIC ${SolarSrc})
target_compile_definitions(Solar PUBLIC YAML_CPP_STATIC_DEFINE)
target_precompile_headers(Solar PRIVATE Solar/solpch.h)

# Imgui
set(ImGuiSrc
  Vendor/imgui/imconfig.h
  Vendor/imgui/imgui_demo.cpp
  Vendor/imgui/imgui_draw.cpp
  Vendor/imgui/imgui_internal.h
  Vendor/imgui/imgui_tables.cpp
  Vendor/imgui/imgui_widgets.cpp
  Vendor/imgui/imgui.cpp
  Vendor/imgui/imgui.h
  Vendor/imgui/imstb_rectpack.h
  Vendor/imgui/imstb_textedit.h
  Vendor/imgui/imstb_truetype.h
  
  # backends
  Vendor/imgui/backends/imgui_impl_glfw.h
  Vendor/imgui/backends/imgui_impl_glfw.cpp
  Vendor/imgui/backends/imgui_impl_opengl3.cpp
  Vendor/imgui/backends/imgui_impl_opengl3.h
  Vendor/imgui/backends/imgui_impl_opengl3_loader.h
)
add_library(ImGui STATIC ${ImGuiSrc})

# ImGuizmo
set(ImGuizmoSrc
  Vendor/ImGuizmo/ImGuizmo.cpp
  Vendor/ImGuizmo/ImGuizmo.h
)
add_library(ImGuizmo STATIC ${ImGuizmoSrc})

# glfw
add_subdirectory(Vendor/glfw/)

# glad
set(gladSrc
  Vendor/glad/src/glad.c
  Vendor/glad/include/glad/glad.h
  Vendor/glad/include/KHR/khrplatform.h
)
add_library(glad STATIC ${gladSrc})

# yaml-cpp
add_subdirectory(Vendor/yaml-cpp/)

# Link
target_link_libraries(Solarium PUBLIC Solar ImGui ImGuizmo glfw glad yaml-cpp)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
  target_link_libraries(Solarium PUBLIC opengl32.lib)
  add_compile_definitions(SOLAR_PLATFORM_WINDOWS)
endif()
