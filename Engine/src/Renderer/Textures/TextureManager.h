#pragma once
#include "Texture.h"
#include "../src/Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API TextureManager {
    public:
        TextureManager();
        void addTexture(const std::string& texturePath);
        std::shared_ptr<Texture> getTexture(const std::string& texturePath) const;
        const std::unordered_map<std::string, std::shared_ptr<Texture>>& getTextureMap() const;

    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureMap;
    };
} // Kita
