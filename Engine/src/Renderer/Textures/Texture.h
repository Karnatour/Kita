#pragma once
#include "../src/Core/DllTemplate.h"
#include <memory>
#include <string>

namespace Kita {
    class KITAENGINE_API Texture {
    public:
        enum class TextureType {
            NONE = 0,
            DIFFUSE = 1,
            SPECULAR = 2,
        };

        virtual ~Texture() = default;
        virtual bool createTexture(const std::filesystem::path& texturePath, const TextureType& textureType) = 0;
        virtual void bind(unsigned int position) = 0;
        static std::shared_ptr<Texture> createPtr();
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
