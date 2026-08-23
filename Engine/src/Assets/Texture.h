#pragma once
#include "../src/Core/DllTemplate.h"
#include <memory>
#include <filesystem>
#include <expected>

#include "Asset.h"
#include "../Renderer/Types/RendererTypes.h"

namespace Kita {
    class KITAENGINE_API Texture : public Asset {
    public:
        enum class TextureType {
            NONE = 0,
            ALBEDO = 1,
            METALLIC_ROUGHNESS = 2,
            CUBEMAP = 3,
            COLOR = 4,
            DEPTH = 5,
            STENCIL = 6,
            SKYBOX = 7,
            NORMAL = 8,
            DEPTH_ARRAY = 9,
        };

        enum class TextureFlags : uint32_t {
            NONE = 0,
            ALBEDO = 1u << 0,
            METALLIC_ROUGHNESS = 1u << 1,
            CUBEMAP = 1u << 2,
            COLOR = 1u << 3,
            DEPTH = 1u << 4,
            STENCIL = 1u << 5,
            SKYBOX = 1u << 6,
            NORMAL = 1u << 7,
            DEPTH_ARRAY = 1u << 8,
        };

        enum class TextureError {
            FILE = 0,
            UNSUPPORTED_NUM_OF_CHANNELS = 1
        };

        virtual ~Texture() = default;
        virtual std::expected<void, TextureError> createTexture(const std::optional<std::filesystem::path>& texturePath, TextureType textureType, std::optional<std::pair<int, int>> resolution) = 0;
        virtual void createBufferTypeTexture(std::pair<int, int> resolution, BufferType bufferType, bool highPrecision) = 0;
        virtual void createBufferTypeTextureArray(std::pair<int, int> resolution, BufferType bufferType, bool highPrecision, int layersCount) = 0;
        virtual void bind(unsigned int position) = 0;
        virtual void destroy() = 0;
        static std::unique_ptr<Texture> createPtr();
        unsigned int getTexture() const;
        int getWidth() const;
        int getHeight() const;
        int getChannels() const;
        const std::optional<std::filesystem::path>& getPath() const;
        TextureType getType() const;
        std::pair<int, int> getResolution() const;

    protected:
        unsigned int m_texture = 0;
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;
        std::optional<std::filesystem::path> m_path;
        TextureType m_textureType = TextureType::NONE;
    };

    constexpr Texture::TextureFlags operator|(Texture::TextureFlags a, Texture::TextureFlags b) {
        return static_cast<Texture::TextureFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    constexpr Texture::TextureFlags operator|=(Texture::TextureFlags& a, const Texture::TextureFlags b) {
        a = a | b;
        return a;
    }
} // Kita
