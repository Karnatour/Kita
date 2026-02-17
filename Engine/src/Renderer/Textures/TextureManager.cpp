#include "../../kitapch.h"
#include "TextureManager.h"

namespace Kita {
    void TextureManager::addTexture(const std::filesystem::path& texturePath, const Texture::TextureType& textureType) {
        if (m_textureMap.contains(texturePath)) {
            KITA_ENGINE_DEBUG("Texture is already included in TextureManager, {}", texturePath.string());
            return;
        }

        auto texture = Texture::createPtr();
        texture->createTexture2D(texturePath, textureType);
        m_textureMap.emplace(texturePath, std::move(texture));
        KITA_ENGINE_DEBUG("Added texture to TextureManager, {}", texturePath.string());
    }

    std::shared_ptr<Texture> TextureManager::getTexture(const std::filesystem::path& texturePath) const {
        auto iterator = m_textureMap.find(texturePath);
        if (iterator != m_textureMap.end()) {
            return iterator->second;
        }
        KITA_ENGINE_ERROR("Texture not found in TextureManager returning nullptr, {}", texturePath.string());
        return nullptr;
    }

    const std::unordered_map<std::filesystem::path, std::shared_ptr<Texture>>& TextureManager::getTextureMap() const {
        return m_textureMap;
    }
} // Kita
