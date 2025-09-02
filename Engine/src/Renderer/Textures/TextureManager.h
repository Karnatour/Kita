#pragma once
#include "Texture.h"
#include "../src/Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API TextureManager {
    public:
        static inline const std::filesystem::path TEXTURE_PREFIX = "../assets/textures/";
        void addTexture(const std::filesystem::path& texturePath, const Texture::TextureType& textureType);
        std::shared_ptr<Texture> getTexture(const std::filesystem::path& texturePath) const;
        const std::unordered_map<std::filesystem::path, std::shared_ptr<Texture>>& getTextureMap() const;

    private:
        std::unordered_map<std::filesystem::path, std::shared_ptr<Texture>> m_textureMap;
    };
} // Kita
