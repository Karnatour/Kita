#include "../../../../kitapch.h"
#include "GLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "../../../../Assets/AssetImporter.h"
#include "../../../../Assets/AssetManager.h"

namespace Kita {
    GLTexture::~GLTexture() {
        if (m_texture != 0) {
            glDeleteTextures(1, &m_texture);
        }
    }

    std::expected<void, Texture::TextureError> GLTexture::createTexture(const std::optional<std::filesystem::path>& texturePath, const TextureType textureType,
                                                                        const std::optional<std::pair<int, int>> resolution) {
        m_path = texturePath;
        m_textureType = textureType;

        switch (textureType) {
            case TextureType::NONE:
                KITA_ENGINE_ASSERT(false, "TextureType::NONE is not a valid selection");
            case TextureType::ALBEDO:
            case TextureType::METALLIC_ROUGHNESS:
                break;
            case TextureType::CUBEMAP:
                return createCubemapTexture(resolution.value());
            case TextureType::COLOR:
            case TextureType::DEPTH:
            case TextureType::STENCIL:
                break;
            case TextureType::SKYBOX:
                return createSkyboxTexture();
            case TextureType::NORMAL:
                break;
            case TextureType::DEPTH_ARRAY:
                KITA_ENGINE_ASSERT(false, "Trying to create DEPTH_ARRAY texture with createTexture() func, use createTextureBufferTypeTextureArray() instead");
        }

        if (!texturePath.has_value()) {
            KITA_ENGINE_ERROR("Texture type: {}, supports only textures with path", magic_enum::enum_name(textureType));
        }

        stbi_set_flip_vertically_on_load(true);
        unsigned char* image = stbi_load((AssetManager::TEXTURE_PREFIX / texturePath.value()).string().c_str(), &m_width, &m_height, &m_channels, 0);

        if (!image) {
            KITA_ENGINE_ERROR("Unable to load texture, {}", texturePath.value().string());
            return std::unexpected(TextureError::FILE);
        }

        const int levels = static_cast<int>(floor(log2(std::max(m_width, m_height))) + 1);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);

#ifdef KITA_BUILD_DEBUG
        glObjectLabel(GL_TEXTURE, m_texture, -1, m_path->string().c_str());
#endif

        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const bool useSRGB = (textureType == TextureType::ALBEDO || textureType == TextureType::COLOR);
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
                KITA_ENGINE_ERROR("Unsupported number of channels for texture, {}", texturePath.value().string());
                return std::unexpected(TextureError::USUPPORTED_NUM_OF_CHANNELS);
        }

        if (textureType == TextureType::ALBEDO || textureType == TextureType::COLOR || textureType == TextureType::METALLIC_ROUGHNESS || textureType == TextureType::NORMAL) {
            glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glGenerateTextureMipmap(m_texture);
        }

        stbi_image_free(image);
        return {};
    }

    std::expected<void, Texture::TextureError> GLTexture::createSkyboxTexture() {
        stbi_set_flip_vertically_on_load(true);
        if (float* image = stbi_loadf((AssetManager::TEXTURE_PREFIX / m_path.value()).string().c_str(), &m_width, &m_height, &m_channels, 0); image) {
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
            KITA_ENGINE_ERROR("Unable to load texture, {}", m_path.value().string());
            return std::unexpected(TextureError::FILE);
        }
        return {};
    }

    std::expected<void, Texture::TextureError> GLTexture::createCubemapTexture(const std::pair<int, int>& resolution) {
        m_width = resolution.first;
        m_height = resolution.second;
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_texture);

        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureStorage2D(m_texture, 1, GL_RGB32F, resolution.first, resolution.second);

        return {};
    }

    void GLTexture::createBufferTypeTexture(const std::pair<int, int> resolution, const BufferType bufferType, const bool highPrecision) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        constexpr float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        glTextureParameterfv(m_texture, GL_TEXTURE_BORDER_COLOR, borderColor);

        switch (bufferType) {
            case BufferType::COLOR:
                m_textureType = TextureType::COLOR;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTextureStorage2D(m_texture, 1, highPrecision ? GL_RGBA32F : GL_RGBA8, resolution.first, resolution.second);
                break;
            case BufferType::DEPTH:
                m_textureType = TextureType::DEPTH;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTextureStorage2D(m_texture, 1, highPrecision ? GL_DEPTH_COMPONENT32F : GL_DEPTH_COMPONENT24, resolution.first, resolution.second);
                break;
            case BufferType::DEPTH_STENCIL:
                m_textureType = TextureType::DEPTH;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTextureStorage2D(m_texture, 1, highPrecision ? GL_DEPTH32F_STENCIL8 : GL_DEPTH24_STENCIL8, resolution.first, resolution.second);
                break;
            default:
                KITA_ENGINE_ERROR("Trying to create unsupported buffertype framebuffer texture {}", magic_enum::enum_name(bufferType));
                break;
        }
    }

    void GLTexture::createBufferTypeTextureArray(const std::pair<int, int> resolution, const BufferType bufferType, const bool highPrecision, const int layersCount) {
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_texture);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        constexpr float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        glTextureParameterfv(m_texture, GL_TEXTURE_BORDER_COLOR, borderColor);

        switch (bufferType) {
            case BufferType::COLOR:
                m_textureType = TextureType::COLOR;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTextureStorage3D(m_texture, 1, highPrecision ? GL_RGBA32F : GL_RGBA8, resolution.first, resolution.second, layersCount);
                break;
            case BufferType::DEPTH:
                m_textureType = TextureType::DEPTH_ARRAY;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTextureStorage3D(m_texture, 1, highPrecision ? GL_DEPTH_COMPONENT32F : GL_DEPTH_COMPONENT24, resolution.first, resolution.second, layersCount);
                break;
            case BufferType::DEPTH_STENCIL:
                m_textureType = TextureType::DEPTH_ARRAY;
                glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTextureStorage3D(m_texture, 1, highPrecision ? GL_DEPTH32F_STENCIL8 : GL_DEPTH24_STENCIL8, resolution.first, resolution.second, layersCount);
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
        m_texture = 0;
    }
} // Kita
