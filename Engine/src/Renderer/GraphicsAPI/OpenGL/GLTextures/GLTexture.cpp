#include "GLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <gli/gli.hpp>

#include "../../../../Assets/AssetImporter.h"
#include "../../../../Assets/AssetManager.h"

namespace Kita {
    GLTexture::~GLTexture() {
        if (m_texture != 0) {
            glDeleteTextures(1, &m_texture);
        }
    }

    std::expected<void, Texture::TextureError> GLTexture::createTexture(std::vector<std::filesystem::path> texturePaths, const TextureType textureType, const std::optional<std::pair<int, int>> resolution) {
        m_paths = std::move(texturePaths);
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
            case TextureType::DISPLACEMENT:
                m_highPrecision = true;
                break;
            case TextureType::DEPTH_ARRAY:
                KITA_ENGINE_ASSERT(false, "Trying to create DEPTH_ARRAY texture with createTexture() func, use createBufferTypeTextureArray() instead"); //TODO Probably merge inside here
            case TextureType::AMBIENT_OCCLUSION:
                break;
        }

        if (m_paths.empty()) {
            return std::unexpected(TextureError{.code = TextureErrorCode::EMPTY, .errorStr = std::string(magic_enum::enum_name(m_textureType))});
        }

        if (m_paths.front().extension() == ".dds") {
            return loadDDSTexture();
        }

        stbi_set_flip_vertically_on_load(true);

        if (auto result = setTexturesInformation(); !result) {
            return result;
        }

        const int levels = static_cast<int>(std::floor(std::log2(std::max(m_width, m_height))) + 1);
        const bool useSRGB = (textureType == TextureType::ALBEDO || textureType == TextureType::COLOR);

        const GLenum internalFormat = getInternalFormat(useSRGB);
        if (m_paths.size() == 1) {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
            glTextureStorage2D(m_texture, levels, internalFormat, m_width, m_height);
        } else {
            glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_texture);
            glTextureStorage3D(m_texture, levels, internalFormat, m_width, m_height, static_cast<GLsizei>(m_paths.size()));
        }

#ifdef KITA_BUILD_DEBUG
        glObjectLabel(GL_TEXTURE, m_texture, -1, m_paths.front().string().c_str());
#endif

        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        const GLenum textureFormat = getTextureFormat();
        const GLenum pixelType = m_highPrecision ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;

        for (size_t i = 0; i < m_paths.size(); ++i) {
            auto& path = m_paths.at(i);

            const std::string fullPath = (AssetManager::TEXTURE_PREFIX / path.string()).string();

            const void* pixels = nullptr;
            int unused_w,unused_h,unused_ch; // stb_image would otherwise crash
            if (m_highPrecision) {
                pixels = stbi_load_16(fullPath.c_str(), &unused_w, &unused_h, &unused_ch, 0);
            } else {
                pixels = stbi_load(fullPath.c_str(), &unused_w, &unused_h, &unused_ch, 0);
            }

            if (!pixels) {
                return std::unexpected(TextureError{.code = TextureErrorCode::FILE, .errorStr = path.string()});
            }

            if (m_paths.size() == 1) {
                glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, textureFormat, pixelType, pixels);
            } else {
                glTextureSubImage3D(m_texture, 0, 0, 0, static_cast<GLint>(i), m_width, m_height, 1, textureFormat, pixelType, pixels);
            }
        }

        if (textureType == TextureType::ALBEDO || textureType == TextureType::COLOR || textureType == TextureType::METALLIC_ROUGHNESS || textureType == TextureType::NORMAL || textureType == TextureType::DISPLACEMENT || textureType ==
            TextureType::AMBIENT_OCCLUSION) {
            glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameterf(m_texture, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
            glGenerateTextureMipmap(m_texture);
        }

        return {};
    }

    std::expected<void, Texture::TextureError> GLTexture::createSkyboxTexture() {
        stbi_set_flip_vertically_on_load(true);

        if (m_paths.size() != 1) {
            return std::unexpected(TextureError({.code = TextureErrorCode::UNSUPPORTED_TEXTURE_ARRAY_FOR_TYPE, .errorStr = m_paths.front().string()}));
        }

        float* image = stbi_loadf((AssetManager::TEXTURE_PREFIX / m_paths.front()).string().c_str(), &m_width, &m_height, &m_channels, 0);
        if (!image) {
            return std::unexpected(TextureError{.code = TextureErrorCode::FILE, .errorStr = m_paths.front().string()});
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);

        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureStorage2D(m_texture, 1, GL_RGB32F, m_width, m_height);
        glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, image);

        stbi_image_free(image);

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

        const int levels = static_cast<int>(std::floor(std::log2(std::max(m_width, m_height))) + 1);
        glTextureStorage2D(m_texture, levels, GL_RGB32F, resolution.first, resolution.second);

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

    void GLTexture::generateMipmaps() {
        glGenerateTextureMipmap(m_texture);
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    void GLTexture::destroy() {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }

    std::expected<void, Texture::TextureError> GLTexture::setTexturesInformation() {
        int previousNumberOfChannels = -1, previousWidth = -1, previousHeight = -1;

        for (size_t i = 0; i < m_paths.size(); ++i) {
            int numberOfChannels = 0, width = 0, height = 0;
            const auto& path = m_paths.at(i);

            const std::string fullPath = (AssetManager::TEXTURE_PREFIX / path.string()).string();

            if (stbi_info(fullPath.c_str(), &width, &height, &numberOfChannels) == 0) {
                return std::unexpected(TextureError{.code = TextureErrorCode::FILE, .errorStr = path.string()});
            }

            if (i != 0 && (previousNumberOfChannels != numberOfChannels || previousWidth != width || previousHeight != height)) {
                return std::unexpected(TextureError{.code = TextureErrorCode::PROPERTIES_MISMATCH, .errorStr = path.string()});
            }

            previousNumberOfChannels = numberOfChannels;
            previousWidth = width;
            previousHeight = height;
        }

        if (previousNumberOfChannels == -1 || previousWidth == -1 || previousHeight == -1) {
            return std::unexpected(TextureError{.code = TextureErrorCode::EMPTY, .errorStr = std::string(magic_enum::enum_name(m_textureType))});
        }

        m_channels = previousNumberOfChannels;
        m_height = previousHeight;
        m_width = previousWidth;

        return {};
    }

    GLenum GLTexture::getInternalFormat(const bool useSRGB) const {
        switch (m_channels) {
            case 1:
                return m_highPrecision ? GL_R16 : GL_R8;
            case 2:
                return m_highPrecision ? GL_RG16 : GL_RG8;
            case 3:
                return m_highPrecision ? GL_RGB16 : (useSRGB ? GL_SRGB8 : GL_RGB8);
            case 4:
                return m_highPrecision ? GL_RGBA16 : (useSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8);
            default:
                KITA_ENGINE_ASSERT(false, "[GLTexture] Unsupported number of channels while getting internalFormat");
                return GL_NONE;
        }
    }

    GLenum GLTexture::getTextureFormat() const {
        switch (m_channels) {
            case 1:
                return GL_RED;
            case 2:
                return GL_RG;
            case 3:
                return GL_RGB;
            case 4:
                return GL_RGBA;
            default:
                KITA_ENGINE_ASSERT(false, "[GLTexture] Unsupported number of channels while getting textureFormat");
                return GL_NONE;
        }
    }

    std::expected<void, Texture::TextureError> GLTexture::loadDDSTexture() {
        if (m_paths.size() != 1) {
            return std::unexpected(TextureError({.code = TextureErrorCode::UNSUPPORTED_TEXTURE_ARRAY_FOR_TYPE, .errorStr = m_paths.front().string()}));
        }

        auto pathStr = m_paths.front().string();
        gli::texture image = gli::load((AssetManager::TEXTURE_PREFIX / pathStr).string());
        if (image.empty()) {
            return std::unexpected(TextureError{.code = TextureErrorCode::FILE, .errorStr = pathStr});
        }

        const gli::gl gl(gli::gl::PROFILE_GL33);
        gli::gl::format format = gl.translate(image.format(), image.swizzles());

        glCreateTextures(gl.translate(image.target()), 1, &m_texture);

#ifdef KITA_BUILD_DEBUG
        glObjectLabel(GL_TEXTURE, m_texture, -1, pathStr.c_str());
#endif

        glTextureParameteri(m_texture, GL_TEXTURE_BASE_LEVEL, 0);
        glTextureParameteri(m_texture, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(image.levels() - 1));

        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, image.levels() > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

        glTextureParameteriv(m_texture, GL_TEXTURE_SWIZZLE_RGBA, &format.Swizzles[0]);

        KITA_ENGINE_DEBUG("DDS {}: GL internal format = 0x{:X}", pathStr, static_cast<GLenum>(format.Internal));

        auto toSRGBInternalFormat = [](const gli::gl::internal_format internalFormat) {
            switch (internalFormat) {
                case gli::gl::INTERNAL_RGBA_DXT1:
                    return gli::gl::INTERNAL_SRGB_DXT1;
                case gli::gl::INTERNAL_RGBA_DXT5:
                    return gli::gl::INTERNAL_SRGB_ALPHA_DXT5;
                default:
                    return internalFormat;
            }
        };

        if (m_textureType == TextureType::ALBEDO || m_textureType == TextureType::COLOR) {
            format.Internal = toSRGBInternalFormat(format.Internal);
        }

        const glm::ivec3 extent(image.extent(0));
        glTextureStorage2D(m_texture, static_cast<GLint>(image.levels()), format.Internal, extent.x, extent.y);

        for (std::size_t level = 0; level < image.levels(); ++level) {
            const auto levelExtent = glm::ivec3(image.extent(level));
            glCompressedTextureSubImage2D(m_texture, static_cast<GLint>(level), 0, 0, levelExtent.x, levelExtent.y, format.Internal, static_cast<GLsizei>(image.size(level)), image.data(0, 0, level));
        }

        return {};
    }
} // Kita
