// Copyright (c) 2024 Sylar129

#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "Core/Debug/Instrumentor.h"

namespace Solar {

namespace {
constexpr GLenum ShaderTypeFromString(const std::string& type) {
  if (type == "vertex") {
    return GL_VERTEX_SHADER;
  }
  if (type == "fragment" || type == "pixel") {
    return GL_FRAGMENT_SHADER;
  }

  SOLAR_CORE_ASSERT(false, "Unknown shader type!");
  return 0;
}
}  // namespace

OpenGLShader::OpenGLShader(const std::string& filepath) {
  SOLAR_PROFILE_FUNCTION();

  std::string source = ReadFile(filepath);
  auto shader_sources = PreProcess(source);
  Compile(shader_sources);

  // Extract name from filepath
  auto last_slash = filepath.find_last_of("/\\");
  last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
  auto last_dot = filepath.rfind('.');
  auto count = last_dot == std::string::npos ? filepath.size() - last_slash
                                             : last_dot - last_slash;
  name_ = filepath.substr(last_slash, count);
}

OpenGLShader::OpenGLShader(const std::string& name,
                           const std::string& vertex_src,
                           const std::string& fragment_src)
    : name_(name) {
  SOLAR_PROFILE_FUNCTION();

  std::unordered_map<GLenum, std::string> sources;
  sources[GL_VERTEX_SHADER] = vertex_src;
  sources[GL_FRAGMENT_SHADER] = fragment_src;
  Compile(sources);
}

OpenGLShader::~OpenGLShader() {
  SOLAR_PROFILE_FUNCTION();

  glDeleteProgram(renderer_id_);
}

void OpenGLShader::Bind() const {
  SOLAR_PROFILE_FUNCTION();

  glUseProgram(renderer_id_);
}

void OpenGLShader::Unbind() const {
  SOLAR_PROFILE_FUNCTION();

  glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string& name, int value) {
  SOLAR_PROFILE_FUNCTION();

  UploadUniformInt(name, value);
}

void OpenGLShader::SetIntArray(const std::string& name, int* values,
                               uint32_t count) {
  SOLAR_PROFILE_FUNCTION();

  UploadUniformIntArray(name, values, count);
}

void OpenGLShader::SetFloat(const std::string& name, float value) {
  SOLAR_PROFILE_FUNCTION();

  UploadUniformFloat(name, value);
}

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
  SOLAR_PROFILE_FUNCTION();

  UploadUniformFloat2(name, value);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
  SOLAR_PROFILE_FUNCTION();

  UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
  SOLAR_PROFILE_FUNCTION();

  UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
  SOLAR_PROFILE_FUNCTION();

  UploadUniformMat4(name, value);
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
  GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform1i(location, value);
}

void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values,
                                         uint32_t count) {
  GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform1iv(location, count, values);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
  GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name,
                                       const glm::vec2& values) {
  GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform2f(location, values.x, values.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name,
                                       const glm::vec3& values) {
  GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform3f(location, values.x, values.y, values.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string& name,
                                       const glm::vec4& values) {
  GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniform4f(location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name,
                                     const glm::mat3& matrix) {
  GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name,
                                     const glm::mat4& matrix) {
  GLint location = glGetUniformLocation(renderer_id_, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string OpenGLShader::ReadFile(const std::string& filepath) {
  SOLAR_PROFILE_FUNCTION();

  std::string result;
  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    result.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(result.data(), result.size());
    in.close();
  } else {
    SOLAR_CORE_ERROR("Could not open file: '{0}'", filepath);
  }
  return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(
    const std::string& source) {
  constexpr char kTypeToken[] = "#type";
  // Including terminators
  constexpr size_t kTypeTokenLength = sizeof(kTypeToken) / sizeof(char);
  SOLAR_PROFILE_FUNCTION();

  std::unordered_map<GLenum, std::string> shader_sources;

  // Start of shader type declaration line
  size_t pos = source.find(kTypeToken, 0);
  while (pos != std::string::npos) {
    // End of shader type declaration line
    size_t eol = source.find_first_of("\r\n", pos);
    SOLAR_CORE_ASSERT(eol != std::string::npos, "Syntax error!");

    // Start of shader type name (after "#type " keyword)
    size_t begin = pos + kTypeTokenLength;
    std::string type = source.substr(begin, eol - begin);
    SOLAR_CORE_ASSERT(ShaderTypeFromString(type),
                      "Invalid shader type specified");

    // Start of shader code after shader type declaration line
    size_t next_line_pos = source.find_first_not_of("\r\n", eol);
    SOLAR_CORE_ASSERT(next_line_pos != std::string::npos, "Syntax error");

    // Start of next shader type declaration line
    pos = source.find(kTypeToken, next_line_pos);
    shader_sources[ShaderTypeFromString(type)] =
        source.substr(next_line_pos, pos - (next_line_pos == std::string::npos
                                                ? source.size() - 1
                                                : next_line_pos));
  }

  return shader_sources;
}

void OpenGLShader::Compile(
    std::unordered_map<GLenum, std::string>& shader_sources) {
  SOLAR_PROFILE_FUNCTION();

  // Get a program object.
  GLuint program = glCreateProgram();

  SOLAR_CORE_ASSERT(shader_sources.size() <= 2,
                    "We only support 2 shaders for now");
  std::array<GLenum, 2> glshader_ids;

  // Compile Shaders
  uint32_t glshader_id_index = 0;
  for (const auto& kv : shader_sources) {
    GLenum shader_type = kv.first;
    const std::string& source = kv.second;

    // Create an empty shader handle
    GLuint shader = glCreateShader(shader_type);

    // Send the shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source_cstr = source.c_str();
    glShaderSource(shader, 1, &source_cstr, 0);

    // Compile the shader
    glCompileShader(shader);

    GLint is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
      GLint max_length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

      // The maxLength includes the NULL character
      std::vector<GLchar> info_log(max_length);
      glGetShaderInfoLog(shader, max_length, &max_length, info_log.data());

      // We don't need the shader anymore.
      glDeleteShader(shader);

      SOLAR_CORE_ERROR("{0}", info_log.data());
      SOLAR_CORE_ASSERT(false, "Shader compilation failure!");
      break;
    }
    // Attach our shaders to our program
    glAttachShader(program, shader);
    glshader_ids[glshader_id_index++] = shader;
  }

  // Link our program
  glLinkProgram(program);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint is_linked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int*)&is_linked);
  if (is_linked == GL_FALSE) {
    GLint max_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

    // The maxLength includes the NULL character
    std::vector<GLchar> info_log(max_length);
    glGetProgramInfoLog(program, max_length, &max_length, info_log.data());

    // We don't need the program anymore.
    glDeleteProgram(program);
    // Don't leak shaders either.
    for (const auto& id : glshader_ids) {
      glDeleteShader(id);
    }

    SOLAR_CORE_ERROR("{0}", info_log.data());
    SOLAR_CORE_ASSERT(false, "Shader link failure!");
    return;
  }

  // Always detach shaders after a successful link.
  for (const auto& id : glshader_ids) {
    glDetachShader(program, id);
    glDeleteShader(id);
  }

  renderer_id_ = program;
}

}  // namespace Solar
