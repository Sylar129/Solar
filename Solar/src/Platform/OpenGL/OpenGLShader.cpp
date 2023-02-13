#include "solpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Solar {

static GLenum ShaderTypeFromString(const std::string& type)
{
    if (type == "vertex") {
        return GL_VERTEX_SHADER;
    }
    if (type == "fragment" || type == "pixel") {
        return GL_FRAGMENT_SHADER;
    }

    SOLAR_CORE_ASSERT(false, "Unknown shader type!");
    return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath)
{
    SOLAR_PROFILE_FUNCTION();

    std::string source = ReadFile(filepath);
    auto shaderSources = PreProcess(source);
    Compile(shaderSources);

    // Extract name from filepath
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    m_Name = filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    : m_Name(name)
{
    SOLAR_PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    Compile(sources);
}

OpenGLShader::~OpenGLShader()
{
    SOLAR_PROFILE_FUNCTION();

    glDeleteProgram(m_RendererID);
}

void OpenGLShader::Bind() const
{
    SOLAR_PROFILE_FUNCTION();

    glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const
{
    SOLAR_PROFILE_FUNCTION();

    glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string& name, int value)
{
    SOLAR_PROFILE_FUNCTION();

    UploadUniformInt(name, value);
}

void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
{
    SOLAR_PROFILE_FUNCTION();

    UploadUniformIntArray(name, values, count);
}

void OpenGLShader::SetFloat(const std::string& name, float value)
{
    SOLAR_PROFILE_FUNCTION();

    UploadUniformFloat(name, value);
}

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
{
    SOLAR_PROFILE_FUNCTION();

    UploadUniformFloat2(name, value);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
{
    SOLAR_PROFILE_FUNCTION();

    UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
{
    SOLAR_PROFILE_FUNCTION();

    UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
{
    SOLAR_PROFILE_FUNCTION();

    UploadUniformMat4(name, value);
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, count, values);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, values.x, values.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, values.x, values.y, values.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string OpenGLShader::ReadFile(const std::string& filepath)
{
    SOLAR_PROFILE_FUNCTION();

    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    } else {
        SOLAR_CORE_ERROR("Could not open file: '{0}'", filepath);
    }
    return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
{
    SOLAR_PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    //Start of shader type declaration line
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos) {
        //End of shader type declaration line
        size_t eol = source.find_first_of("\r\n", pos);
        SOLAR_CORE_ASSERT(eol != std::string::npos, "Syntax error!");

        //Start of shader type name (after "#type " keyword)
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);
        SOLAR_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

        //Start of shader code after shader type declaration line
        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        SOLAR_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");

        //Start of next shader type declaration line
        pos = source.find(typeToken, nextLinePos);
        shaderSources[ShaderTypeFromString(type)] =
            source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
    }

    return shaderSources;
}

void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
{
    SOLAR_PROFILE_FUNCTION();

    // Get a program object.
    GLuint program = glCreateProgram();

    SOLAR_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
    std::array<GLenum, 2> glShaderIDs;

    // Compile Shaders
    uint32_t glShaderIDIndex = 0;
    for (auto& kv : shaderSources) {
        GLenum shaderType = kv.first;
        const std::string& source = kv.second;

        // Create an empty shader handle
        GLuint shader = glCreateShader(shaderType);

        // Send the shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        // Compile the shader
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(shader);

            SOLAR_CORE_ERROR("{0}", infoLog.data());
            SOLAR_CORE_ASSERT(false, "Shader compilation failure!");
            break;
        }
        // Attach our shaders to our program
        glAttachShader(program, shader);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        for (auto id : glShaderIDs) {
            glDeleteShader(id);
        }

        SOLAR_CORE_ERROR("{0}", infoLog.data());
        SOLAR_CORE_ASSERT(false, "Shader link failure!");
        return;
    }

    // Always detach shaders after a successful link.
    for (auto id : glShaderIDs) {
        glDetachShader(program, id);
        glDeleteShader(id);
    }

    m_RendererID = program;
}

} // namespace Solar
