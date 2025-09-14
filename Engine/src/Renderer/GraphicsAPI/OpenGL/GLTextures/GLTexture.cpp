#include "../../../../kitapch.h"
#include "GLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

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

            switch (m_channels) {
                case 3:
                    glTextureStorage2D(m_texture, levels, GL_RGB8, m_width, m_height);
                    glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, image);
                    break;
                case 4:
                    glTextureStorage2D(m_texture, levels, GL_RGBA8, m_width, m_height);
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

            glTextureStorage2D(m_texture, 1, GL_RGB16F, m_width, m_height);
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
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_texture);

        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureStorage2D(m_texture, 1, GL_RGB16F, resolution.first, resolution.second);
    }

    void GLTexture::createBufferTypeTexture(const BufferType& bufferType, const std::pair<int, int>& resolution) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        switch (bufferType) {
            case BufferType::COLOR:
                m_textureType = TextureType::COLOR;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTextureStorage2D(m_texture, 1, GL_RGB8, resolution.first, resolution.second);
                break;
            case BufferType::DEPTH:
                m_textureType = TextureType::DEPTH;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTextureStorage2D(m_texture, 1, GL_DEPTH_COMPONENT24, resolution.first, resolution.second);
                break;
            case BufferType::STENCIL:
                m_textureType = TextureType::STENCIL;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTextureStorage2D(m_texture, 1, GL_STENCIL_INDEX8, resolution.first, resolution.second);
                break;
        }
    }

    void GLTexture::bind(const unsigned int position) {
        glBindTextureUnit(position, m_texture);
    }
} // Kita
