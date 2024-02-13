// Copyright (c) 2024 Sylar129

#include "Core/Renderer/Shader.h"

#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Solar {

Ref<Shader> Shader::Create(const std::string& filepath) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::kNone:
      SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::kOpenGL:
      return CreateRef<OpenGLShader>(filepath);
  }
  SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

Ref<Shader> Shader::Create(const std::string& name,
                           const std::string& vertex_src,
                           const std::string& fragment_src) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::kNone:
      SOLAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::kOpenGL:
      return CreateRef<OpenGLShader>(name, vertex_src, fragment_src);
  }
  SOLAR_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
  SOLAR_CORE_ASSERT(!Exists(name), "Shader already exists!");
  shaders_[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader) {
  const auto& name = shader->GetName();
  Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
  auto shader = Shader::Create(filepath);
  Add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name,
                                const std::string& filepath) {
  auto shader = Shader::Create(filepath);
  Add(name, shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name) {
  SOLAR_CORE_ASSERT(Exists(name), "Shader not found!");
  return shaders_[name];
}

bool ShaderLibrary::Exists(const std::string& name) const {
  return shaders_.find(name) != shaders_.end();
}

}  // namespace Solar
