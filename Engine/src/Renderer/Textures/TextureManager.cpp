#include "../../kitapch.h"
#include "TextureManager.h"

namespace Kita {
    void TextureManager::addTexture(const std::filesystem::path& texturePath, const Texture::TextureType& textureType) {
        auto [iterator,inserted] = m_textureMap.try_emplace(texturePath, Texture::createPtr());

        if (inserted) {
            if (iterator->second->createTexture2D(texturePath, textureType)) {
                KITA_ENGINE_DEBUG("Added texture to TextureManager, {}", texturePath.string());
            }
            else {
                m_textureMap.erase(iterator);
            }
        }
        else {
            KITA_ENGINE_WARN("Texture is already included in TextureManager, {}", texturePath.string());
        }
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
