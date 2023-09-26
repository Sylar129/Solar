cmake_minimum_required(VERSION 3.20)

project(Solarium)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
  ./
  ../Vendor/imgui/
  ../Vendor/glfw/include/
  ../Vendor/glad/include/
  ../Vendor/spdlog/include/
  ../Vendor/glm/
  ../Vendor/entt/src/
  ../Vendor/ImGuizmo/
  ../Vendor/yaml-cpp/include/
  ../Vendor/stb/
)

# Solar
set(SolarSrc
  Solar.h
  Core/Base/Application.cpp
  Core/Base/Application.h
  Core/Base/EntryPoint.h
  Core/Base/Input.h
  Core/Base/KeyCodes.h
  Core/Base/Layer.cpp
  Core/Base/Layer.h
  Core/Base/LayerStack.cpp
  Core/Base/LayerStack.h
  Core/Base/Log.cpp
  Core/Base/Log.h
  Core/Base/MouseCodes.h
  Core/Base/Sundry.h
  Core/Base/Timestep.h
  Core/Base/Window.h
  Core/Debug/Instrumentor.h
  Core/Events/ApplicationEvent.h
  Core/Events/Event.h
  Core/Events/KeyEvent.h
  Core/Events/MouseEvent.h
  Core/ImGui/ImGuiLayer.cpp
  Core/ImGui/ImGuiLayer.h
  Core/Math/Math.cpp
  Core/Math/Math.h
  Core/Renderer/Buffer.cpp
  Core/Renderer/Buffer.h
  Core/Renderer/Camera.h
  Core/Renderer/EditorCamera.cpp
  Core/Renderer/EditorCamera.h
  Core/Renderer/Framebuffer.cpp
  Core/Renderer/Framebuffer.h
  Core/Renderer/GraphicsContext.h
  Core/Renderer/OrthographicCamera.cpp
  Core/Renderer/OrthographicCamera.h
  Core/Renderer/OrthographicCameraController.cpp
  Core/Renderer/OrthographicCameraController.h
  Core/Renderer/RenderCommand.cpp
  Core/Renderer/RenderCommand.h
  Core/Renderer/Renderer.cpp
  Core/Renderer/Renderer.h
  Core/Renderer/Renderer2D.cpp
  Core/Renderer/Renderer2D.h
  Core/Renderer/RendererAPI.cpp
  Core/Renderer/RendererAPI.h
  Core/Renderer/Shader.cpp
  Core/Renderer/Shader.h
  Core/Renderer/SubTexture2D.cpp
  Core/Renderer/SubTexture2D.h
  Core/Renderer/Texture.cpp
  Core/Renderer/Texture.h
  Core/Renderer/VertexArray.cpp
  Core/Renderer/VertexArray.h
  Core/Scene/Components.h
  Core/Scene/Entity.cpp
  Core/Scene/Entity.h
  Core/Scene/Scene.cpp
  Core/Scene/Scene.h
  Core/Scene/SceneCamera.cpp
  Core/Scene/SceneCamera.h
  Core/Scene/SceneSerializer.cpp
  Core/Scene/SceneSerializer.h
  Core/Scene/ScriptableEntity.h
  Core/Utils/PlatformUtils.h

  Platform/OpenGL/OpenGLBuffer.cpp
  Platform/OpenGL/OpenGLBuffer.h
  Platform/OpenGL/OpenGLContext.cpp
  Platform/OpenGL/OpenGLContext.h
  Platform/OpenGL/OpenGLFramebuffer.cpp
  Platform/OpenGL/OpenGLFramebuffer.h
  Platform/OpenGL/OpenGLRendererAPI.cpp
  Platform/OpenGL/OpenGLRendererAPI.h
  Platform/OpenGL/OpenGLShader.cpp
  Platform/OpenGL/OpenGLShader.h
  Platform/OpenGL/OpenGLTexture.cpp
  Platform/OpenGL/OpenGLTexture.h
  Platform/OpenGL/OpenGLVertexArray.cpp
  Platform/OpenGL/OpenGLVertexArray.h

  Platform/Windows/WindowsInput.cpp
  Platform/Windows/WindowsPlatformUtils.cpp
  Platform/Windows/WindowsWindow.cpp
  Platform/Windows/WindowsWindow.h
)
add_library(Solar STATIC ${SolarSrc})
target_compile_definitions(Solar PUBLIC YAML_CPP_STATIC_DEFINE)
target_precompile_headers(Solar PRIVATE solpch.h)

# Imgui
set(ImGuiSrc
  ../Vendor/imgui/imconfig.h
  ../Vendor/imgui/imgui_demo.cpp
  ../Vendor/imgui/imgui_draw.cpp
  ../Vendor/imgui/imgui_internal.h
  ../Vendor/imgui/imgui_tables.cpp
  ../Vendor/imgui/imgui_widgets.cpp
  ../Vendor/imgui/imgui.cpp
  ../Vendor/imgui/imgui.h
  ../Vendor/imgui/imstb_rectpack.h
  ../Vendor/imgui/imstb_textedit.h
  ../Vendor/imgui/imstb_truetype.h
  
  # backends
  ../Vendor/imgui/backends/imgui_impl_glfw.h
  ../Vendor/imgui/backends/imgui_impl_glfw.cpp
  ../Vendor/imgui/backends/imgui_impl_opengl3.cpp
  ../Vendor/imgui/backends/imgui_impl_opengl3.h
  ../Vendor/imgui/backends/imgui_impl_opengl3_loader.h
)
add_library(ImGui STATIC ${ImGuiSrc})

# ImGuizmo
set(ImGuizmoSrc
  ../Vendor/ImGuizmo/ImGuizmo.cpp
  ../Vendor/ImGuizmo/ImGuizmo.h
)
add_library(ImGuizmo STATIC ${ImGuizmoSrc})

# glfw
add_subdirectory(../Vendor/glfw/ glfwlib/)

# glad
set(gladSrc
  ../Vendor/glad/src/glad.c
  ../Vendor/glad/include/glad/glad.h
  ../Vendor/glad/include/KHR/khrplatform.h
)
add_library(glad STATIC ${gladSrc})

# yaml-cpp
add_subdirectory(../Vendor/yaml-cpp/ yamllib/)

target_link_libraries(Solar PUBLIC ImGui ImGuizmo glfw glad yaml-cpp)
