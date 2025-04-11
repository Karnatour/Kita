#include "TextureManager.h"

namespace Kita {
    TextureManager::TextureManager() {
        addTexture("../assets/texture/wood_floor.jpg");
    }

    void TextureManager::addTexture(const std::string& texturePath) {
        auto [iterator,inserted] = m_textureMap.try_emplace(texturePath, Texture::createPtr());

        if (inserted) {
            iterator->second->createTexture(texturePath);
            KITA_ENGINE_DEBUG("Added texture to TextureManager, {}", texturePath.c_str());
        }
        else {
            KITA_ENGINE_WARN("Texture is already included in TextureManager, {}", texturePath.c_str());
        }
    }

    std::shared_ptr<Texture> TextureManager::getTexture(const std::string& texturePath) const {
        auto iterator = m_textureMap.find(texturePath);
        if (iterator != m_textureMap.end()) {
            return iterator->second;
        }
        KITA_ENGINE_ERROR("Texture not found in TextureManager returning nullptr, {}", texturePath.c_str());
        return nullptr;
    }

    const std::unordered_map<std::string, std::shared_ptr<Texture>>& TextureManager::getTextureMap() const {
        return m_textureMap;
    }
} // Kita