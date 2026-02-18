#include "../../../../kitapch.h"
#include "GLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <magic_enum/magic_enum.hpp>

#include "../../../../Asset/AssetImporter.h"
#include "../../../Textures/TextureManager.h"

namespace Kita {
    GLTexture::~GLTexture() {
        glDeleteTextures(1, &m_texture);
    }

    bool GLTexture::createTexture2D(const std::filesystem::path& texturePath, const TextureType& textureType) {
        m_path = texturePath;
        m_textureType = textureType;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* image = stbi_load((TextureManager::TEXTURE_PREFIX / texturePath).string().c_str(), &m_width, &m_height, &m_channels, 0);
        if (image) {
            int levels = static_cast<int>(floor(log2(std::max(m_width, m_height))) + 1);
            glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);

            //TODO: Support parameter option
            glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            const bool useSRGB = (textureType == TextureType::DIFFUSE || textureType == TextureType::COLOR);
            switch (m_channels) {
                case 2:
                    glTextureStorage2D(m_texture, levels, GL_RG8, m_width, m_height);
                    glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RG, GL_UNSIGNED_BYTE, image);
                    break;
                case 3:
                    glTextureStorage2D(m_texture, levels, useSRGB ? GL_SRGB8 : GL_RGB8, m_width, m_height);
                    glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, image);
                    break;
                case 4:
                    glTextureStorage2D(m_texture, levels, useSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8, m_width, m_height);
                    glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, image);
                    break;
                default:
                    KITA_ENGINE_ERROR("Unsupported number of channels for texture, {}", texturePath.string());
                    return false;
            }

            glGenerateTextureMipmap(m_texture);
            stbi_image_free(image);
        }
        else {
            KITA_ENGINE_ERROR("Unable to load texture, {}", texturePath.string());
            return false;
        }
        return true;
    }

    bool GLTexture::createSkyboxTexture2D(const std::filesystem::path& texturePath) {
        m_path = texturePath;
        m_textureType = TextureType::DIFFUSE;
        stbi_set_flip_vertically_on_load(true);
        float* image = stbi_loadf((TextureManager::TEXTURE_PREFIX / texturePath).string().c_str(), &m_width, &m_height, &m_channels, 0);
        if (image) {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);

            glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTextureStorage2D(m_texture, 1, GL_RGB32F, m_width, m_height);
            glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, image);

            stbi_image_free(image);
        }
        else {
            KITA_ENGINE_ERROR("Unable to load texture, {}", texturePath.string());
            return false;
        }
        return true;
    }

    void GLTexture::createCubemapTexture(const std::pair<int, int>& resolution) {
        m_textureType = TextureType::CUBEMAP;
        m_width = resolution.first;
        m_height = resolution.second;
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_texture);

        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureStorage2D(m_texture, 1, GL_RGB32F, resolution.first, resolution.second);
    }

    void GLTexture::createBufferTypeTexture(const std::pair<int, int>& resolution, const BufferType& bufferType, const bool highPrecision) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        glTextureParameterfv(m_texture, GL_TEXTURE_BORDER_COLOR, borderColor);

        const auto colorInternalFormat = highPrecision ? GL_RGBA32F : GL_RGBA8;

        switch (bufferType) {
            case BufferType::COLOR:
                m_textureType = TextureType::COLOR;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTextureStorage2D(m_texture, 1, colorInternalFormat, resolution.first, resolution.second);
                break;
            case BufferType::DEPTH:
                m_textureType = TextureType::DEPTH;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTextureStorage2D(m_texture, 1, GL_DEPTH_COMPONENT24, resolution.first, resolution.second);
                break;
            case BufferType::DEPTH_STENCIL:
                m_textureType = TextureType::DEPTH;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTextureStorage2D(m_texture, 1, GL_DEPTH24_STENCIL8, resolution.first, resolution.second);
                break;
            default:
                KITA_ENGINE_ERROR("Trying to create unsupported buffertype framebuffer texture {}", magic_enum::enum_name(bufferType));
                break;
        }
    }

    void GLTexture::bind(const unsigned int position) {
        glBindTextureUnit(position, m_texture);
    }

    void GLTexture::destroy() {
        glDeleteTextures(1, &m_texture);
    }
} // Kita
