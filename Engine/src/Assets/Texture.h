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
            DIFFUSE = 1,
            SPECULAR = 2,
            CUBEMAP = 3,
            COLOR = 4,
            DEPTH = 5,
            STENCIL = 6,
            SKYBOX = 7
        };

        enum class TextureError {
            FILE = 0,
            GRAPHICS_API = 1
        };

        virtual ~Texture() = default;
        virtual std::expected<void,TextureError> createTexture(const std::filesystem::path& texturePath, TextureType textureType, std::optional<std::pair<int, int>> resolution = std::nullopt) = 0;
        virtual bool createSkyboxTexture2D(const std::filesystem::path& texturePath) = 0;
        virtual void createCubemapTexture(const std::pair<int, int>& resolution) = 0;
        virtual void createBufferTypeTexture(std::pair<int, int> resolution, BufferType bufferType, bool highPrecision) = 0;
        virtual void bind(unsigned int position) = 0;
        virtual void destroy() = 0;
        static std::unique_ptr<Texture> createPtr();
        unsigned int getTexture() const;
        int getWidth() const;
        int getHeight() const;
        int getChannels() const;
        const std::filesystem::path& getPath() const;
        TextureType getType() const;

    protected:
        unsigned int m_texture = 0;
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;
        std::filesystem::path m_path;
        TextureType m_textureType = TextureType::NONE;
    };
} // Kita
