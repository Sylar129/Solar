#include "solpch.h"
#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Solar {
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
      : m_Width(width), m_Height(height), m_InternaleFormat(GL_RGBA8), m_DataFormat(GL_RGBA) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternaleFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
      : m_Path(path) {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        SOLAR_CORE_ASSERT(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0;
        GLenum dataFormat = 0;

        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        m_InternaleFormat = internalFormat;
        m_DataFormat = dataFormat;

        SOLAR_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);


        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size) {
        uint32_t bytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
        SOLAR_CORE_ASSERT(size == m_Width * m_Height * bytesPerPixel, "Data nust be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const {
        glBindTextureUnit(slot, m_RendererID);
    }
}
