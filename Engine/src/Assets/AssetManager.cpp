#include "AssetManager.h"

#include "../Renderer/Properties/LightShadowProperties.h"
#include "../Renderer/Util/LightUtil.h"

namespace Kita {
    void AssetManager::addDefaultAssets() {
        createAsset<Texture>(DEFAULT_TEXTURE, {.setAsDefault = true}, Texture::TextureType::ALBEDO, std::nullopt);
        createAsset<Shader>(std::nullopt, {.setAsDefault = true}, std::initializer_list{
                                Shader::vert(DEFAULT_VERTEX),
                                Shader::frag(DEFAULT_FRAGMENT, {Shader::define("MAX_CASCADES", std::to_string(DirectionalShadowProperties::MAX_CASCADES))})
                            });
        createAsset<Mesh>(std::nullopt, {.setAsDefault = true}, Geometry::getTriangleData());
    }

    AssetManager::AssetID AssetManager::getNextID() {
        static AssetID IDCounter = 0;
        return ++IDCounter;
    }

    std::optional<AssetManager::AssetID> AssetManager::getIDForStringPath(const std::string& string) {
        if (const auto it = m_stringPathToID.find(string); it != m_stringPathToID.end()) {
            return it->second;
        }
        KITA_ENGINE_ERROR("StringPath is not stored {}", string);
        return std::nullopt;
    }

    AssetManager::AssetID AssetManager::getOrAddStringPath(const std::string& string) {
        if (const auto it = m_stringPathToID.find(string); it != m_stringPathToID.end()) {
            KITA_ENGINE_INFO("StringPath: {} is already stored, returning original", string);
            return it->second;
        }

        auto id = getNextID();
        m_stringPathToID.try_emplace(string, id);
        return id;
    }
} // Kita
