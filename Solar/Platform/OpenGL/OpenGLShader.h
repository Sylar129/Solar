#pragma once

#include "Core/Renderer/Shader.h"

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace Solar {

class OpenGLShader : public Shader {
 public:
  explicit OpenGLShader(const std::string& filepath);
  OpenGLShader(const std::string& name, const std::string& vertex_src,
               const std::string& fragment_src);
  virtual ~OpenGLShader();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void SetInt(const std::string& name, int value) override;
  virtual void SetIntArray(const std::string& name, int* values,
                           uint32_t count) override;
  virtual void SetFloat(const std::string& name, float value) override;
  virtual void SetFloat2(const std::string& name,
                         const glm::vec2& value) override;
  virtual void SetFloat3(const std::string& name,
                         const glm::vec3& value) override;
  virtual void SetFloat4(const std::string& name,
                         const glm::vec4& value) override;
  virtual void SetMat4(const std::string& name,
                       const glm::mat4& value) override;

  virtual const std::string& GetName() const override { return name_; }

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
  std::string ReadFile(const std::string& filepath);
  std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
  void Compile(std::unordered_map<GLenum, std::string>& shader_sources);

  uint32_t renderer_id_ = 0;
  std::string name_;
};

}  // namespace Solar
