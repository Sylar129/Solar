#include "solpch.h"

#include "Core/Base/Timer.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Solar {

namespace Utils {

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

static constexpr shaderc_shader_kind GLShaderStageToShaderc(GLenum stage) {
    switch (stage) {
    case GL_VERTEX_SHADER:
        return shaderc_glsl_vertex_shader;
    case GL_FRAGMENT_SHADER:
        return shaderc_glsl_fragment_shader;
    }
    SOLAR_CORE_ASSERT(false, "Unknown stage!");
}

static constexpr auto GLShaderStageToString(GLenum stage)
{
    switch (stage) {
    case GL_VERTEX_SHADER:
        return "GL_VERTEX_SHADER";
    case GL_FRAGMENT_SHADER:
        return "GL_FRAGMENT_SHADER";
    }
    SOLAR_CORE_ASSERT(false, "Unknown stage!");
}

static constexpr auto GetCacheDirectory()
{
    // TODO: make sure the assets directory is valid
    return "assets/cache/shader/opengl";
}

static void CreateCacheDirectoryIfNeeded()
{
    const auto cacheDirectory = GetCacheDirectory();
    if (!std::filesystem::exists(cacheDirectory)) {
        std::filesystem::create_directories(cacheDirectory);
    }
}

static constexpr auto GLShaderStageCachedOpenGLFileExtension(GLenum stage)
{
    switch (stage) {
    case GL_VERTEX_SHADER:
        return ".cached_opengl.vert";
    case GL_FRAGMENT_SHADER:
        return ".cached_opengl.frag";
    }
    SOLAR_CORE_ASSERT(false, "Unknown stage!");
}

static constexpr auto GLShaderStageCachedVulkanFileExtension(GLenum stage)
{
    switch (stage) {
    case GL_VERTEX_SHADER:
        return ".cached_vulkan.vert";
    case GL_FRAGMENT_SHADER:
        return ".cached_vulkan.frag";
    }
    SOLAR_CORE_ASSERT(false, "Unknown stage!");
}

} // namespace Utils

OpenGLShader::OpenGLShader(const std::string& filepath) : m_FilePath(filepath)
{
    SOLAR_PROFILE_FUNCTION();

    Utils::CreateCacheDirectoryIfNeeded();

    std::string source = ReadFile(filepath);
    auto shaderSources = PreProcess(source);

    {
        Timer timer;
        CompileOrGetVulkanBinaries(shaderSources);
        CompileOrGetOpenGLBInaries();
        CreateProgram();
        SOLAR_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
    }

    // Extract name from filepath
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash
                                              : lastDot - lastSlash;
    m_Name = filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(const std::string& name,
                           const std::string& vertexSrc,
                           const std::string& fragmentSrc)
    : m_Name(name)
{
    SOLAR_PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    CompileOrGetVulkanBinaries(sources);
    CompileOrGetOpenGLBInaries();
    CreateProgram();
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

void OpenGLShader::SetIntArray(const std::string& name, int* values,
                               uint32_t count)
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

void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values,
                                         uint32_t count)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, count, values);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name,
                                       const glm::vec2& values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, values.x, values.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name,
                                       const glm::vec3& values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, values.x, values.y, values.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string& name,
                                       const glm::vec4& values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name,
                                     const glm::mat3& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name,
                                     const glm::mat4& matrix)
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

TShaderSources OpenGLShader::PreProcess(const std::string& source)
{
    SOLAR_PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    // Start of shader type declaration line
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos) {
        // End of shader type declaration line
        size_t eol = source.find_first_of("\r\n", pos);
        SOLAR_CORE_ASSERT(eol != std::string::npos, "Syntax error!");

        // Start of shader type name (after "#type " keyword)
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);
        SOLAR_CORE_ASSERT(Utils::ShaderTypeFromString(type),
                          "Invalid shader type specified");

        // Start of shader code after shader type declaration line
        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        SOLAR_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");

        // Start of next shader type declaration line
        pos = source.find(typeToken, nextLinePos);
        shaderSources[Utils::ShaderTypeFromString(type)] =
            source.substr(nextLinePos, pos - (nextLinePos == std::string::npos
                                                  ? source.size() - 1
                                                  : nextLinePos));
    }

    return shaderSources;
}

void OpenGLShader::CompileOrGetVulkanBinaries(
    const TShaderSources& shaderSources)
{
    SOLAR_PROFILE_FUNCTION();

    GLuint program = glCreateProgram();

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan,
                                 shaderc_env_version_vulkan_1_2);
    constexpr bool optimize = true;
    if constexpr (optimize) {
        options.SetOptimizationLevel(shaderc_optimization_level_performance);
    }

    std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

    auto& shaderData = m_VulkanSPIRV;
    shaderData.clear();
    for (auto&& [stage, source] : shaderSources) {
        std::filesystem::path shaderFilePath = m_FilePath;
        std::filesystem::path cachedPath =
            cacheDirectory /
            (shaderFilePath.filename().string() +
             Utils::GLShaderStageCachedVulkanFileExtension(stage));

        std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
        if (in.is_open()) {
            in.seekg(0, std::ios::end);
            auto size = in.tellg();
            in.seekg(0, std::ios::beg);

            auto& data = shaderData[stage];
            data.resize(size / sizeof(uint32_t));
            in.read((char*)data.data(), size);
        } else {
            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
                source, Utils::GLShaderStageToShaderc(stage),
                m_FilePath.c_str(), options);
            if (module.GetCompilationStatus() !=
                shaderc_compilation_status_success) {
                SOLAR_CORE_ERROR(module.GetErrorMessage());
                SOLAR_CORE_ASSERT(false, "Shaderc compile failed!");
            }

            shaderData[stage] =
                std::vector<uint32_t>(module.cbegin(), module.cend());

            std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
            if (out.is_open()) {
                auto& data = shaderData[stage];
                out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                out.flush();
                out.close();
            }
        }
    }

    for (auto&& [stage, data] : shaderData) {
        Reflect(stage, data);
    }
}

void OpenGLShader::CompileOrGetOpenGLBInaries()
{
    SOLAR_PROFILE_FUNCTION();

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_opengl,
                                 shaderc_env_version_opengl_4_5);
    constexpr bool optimize = true;
    if constexpr (optimize) {
        options.SetOptimizationLevel(shaderc_optimization_level_performance);
    }

    std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

    auto& shaderData = m_OpenGLSPIRV;
    shaderData.clear();
    m_OpenGLSourceCode.clear();
    for (auto&& [stage, spirv] : m_VulkanSPIRV) {
        std::filesystem::path shaderFilePath = m_FilePath;
        std::filesystem::path cachedPath =
            cacheDirectory /
            (shaderFilePath.filename().string() +
             Utils::GLShaderStageCachedOpenGLFileExtension(stage));

        std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
        if (in.is_open()) {
            in.seekg(0, std::ios::end);
            auto size = in.tellg();
            in.seekg(0, std::ios::beg);

            auto& data = shaderData[stage];
            data.resize(size / sizeof(uint32_t));
            in.read((char*)data.data(), size);
        } else {
            spirv_cross::CompilerGLSL glslCompiler(spirv);
            m_OpenGLSourceCode[stage] = glslCompiler.compile();
            auto& source = m_OpenGLSourceCode[stage];

            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
                source, Utils::GLShaderStageToShaderc(stage),
                m_FilePath.c_str(),options);
            if (module.GetCompilationStatus() !=
                shaderc_compilation_status_success) {
                SOLAR_CORE_ERROR(module.GetErrorMessage());
                SOLAR_CORE_ASSERT(false, "Shaderc compile failed!");
            }

            shaderData[stage] =
                std::vector<uint32_t>(module.cbegin(), module.cend());

            std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
            if (out.is_open()) {
                auto& data = shaderData[stage];
                out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                out.flush();
                out.close();
            }
        }
    }
}

void OpenGLShader::CreateProgram()
{
    GLuint program = glCreateProgram();
    std::vector<GLuint> shaderIDs;

    for (auto&& [stage, spirv] : m_OpenGLSPIRV) {
        GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
        glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V,
                       spirv.data(), spirv.size() * sizeof(uint32_t));
        glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
        glAttachShader(program, shaderID);
    }

    glLinkProgram(program);
    GLint isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        SOLAR_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath,
                         infoLog.data());
        glDeleteProgram(program);
        for (auto id : shaderIDs) {
            glDeleteShader(id);
        }
    }

    for (auto id : shaderIDs) {
        glDetachShader(program, id);
        glDeleteShader(program);
    }

    m_RendererID = program;
}

void OpenGLShader::Reflect(GLenum stage,
                           const std::vector<uint32_t>& shaderData)
{
    spirv_cross::Compiler compiler(shaderData);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    SOLAR_CORE_TRACE("OpenGLShader::Reflect - {0} {1}",
                     Utils::GLShaderStageToString(stage), m_FilePath);
    SOLAR_CORE_TRACE("    {0} uniform buffers",
                     resources.uniform_buffers.size());
    SOLAR_CORE_TRACE("    {0} resources",
                     resources.sampled_images.size());

    SOLAR_CORE_TRACE("Uniform buffers:");
    for (const auto& resource : resources.uniform_buffers) {
        const auto& bufferType = compiler.get_type(resource.base_type_id);
        auto bufferSize = compiler.get_declared_struct_size(bufferType);
        auto binding =
            compiler.get_decoration(resource.id, spv::DecorationBinding);
        auto memberCount = bufferType.member_types.size();

        SOLAR_CORE_TRACE("  {0}", resource.name);
        SOLAR_CORE_TRACE("    Size = {0}", bufferSize);
        SOLAR_CORE_TRACE("    Binding = {0}", binding);
        SOLAR_CORE_TRACE("    Members = {0}", memberCount);
    }
}

} // namespace Solar
