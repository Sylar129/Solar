// Copyright (c) 2024 Sylar129

#pragma once

#include <string>

#include "core/renderer/shader.h"

namespace solar {

class OpenGLShader : public Shader {
 public:
  explicit OpenGLShader(const std::string& filepath);
  OpenGLShader(const std::string& name, const std::string& vertex_src,
               const std::string& fragment_src);
  ~OpenGLShader() override;

  void Bind() const override;
  void Unbind() const override;

  void SetInt(const std::string& name, int value) override;
  void SetIntArray(const std::string& name, int* values,
                   uint32_t count) override;
  void SetFloat(const std::string& name, float value) override;
  void SetFloat2(const std::string& name, const glm::vec2& value) override;
  void SetFloat3(const std::string& name, const glm::vec3& value) override;
  void SetFloat4(const std::string& name, const glm::vec4& value) override;
  void SetMat4(const std::string& name, const glm::mat4& value) override;

  const std::string& GetName() const override { return name_; }

  void UploadUniformInt(const std::string& name, int value);
  void UploadUniformIntArray(const std::string& name, int* values,
                             uint32_t count);
  void UploadUniformFloat(const std::string& name, float value);
  void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
  void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
  void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

  void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
  void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

 private:
  uint32_t renderer_id_ = 0;
  std::string name_;
};

}  // namespace solar
