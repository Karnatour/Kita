#include "AssetManager.h"

#include "../Renderer/Properties/LightShadowProperties.h"
#include "../Renderer/Util/GeometryUtil.h"
#include "../Renderer/Util/LightUtil.h"

namespace Kita {
    void AssetManager::addDefaultAssets() {
        getOrCreateAsset<Texture>(AssetKey{DEFAULT_TEXTURE}, {.replace = true, .setAsDefault = true}, Texture::TextureType::ALBEDO, std::nullopt);
        getOrCreateAsset<Shader>(AssetKey{DEFAULT_VERTEX, DEFAULT_FRAGMENT}, {.replace = true, .setAsDefault = true},
                                 std::vector{Shader::vert(), Shader::frag({Shader::define("MAX_CASCADES", std::to_string(DirectionalShadowProperties::MAX_CASCADES))})});
        getOrCreateAsset<Mesh>(AssetKey{Geometry::getTriangleKey()}, {.replace = true, .setAsDefault = true}, Geometry::getTriangleData());
    }

    std::optional<AssetManager::AssetID> AssetManager::getAssetID(const AssetOptions options, const std::string& pathString) {
        std::optional<AssetID> ID;
        if (options.setAsDefault) {
            m_stringPathToID.insert_or_assign(pathString, DEFAULT_ASSET_ID);
            ID = DEFAULT_ASSET_ID;
        } else {
            ID = options.replace ? getIDForStringPath(pathString) : getOrAddStringPath(pathString);
        }
        return ID;
    }

    AssetManager::AssetID AssetManager::getNextID() {
        static AssetID IDCounter = 0;
        return ++IDCounter;
    }

    std::optional<AssetManager::AssetID> AssetManager::getIDForStringPath(const std::string& string) {
        if (const auto it = m_stringPathToID.find(string); it != m_stringPathToID.end()) {
            return it->second;
        }
        KITA_ENGINE_ERROR("[AssetBuilder] StringPath is not stored {}", string);
        return std::nullopt;
    }

    AssetManager::AssetID AssetManager::getOrAddStringPath(const std::string& string) {
        if (const auto it = m_stringPathToID.find(string); it != m_stringPathToID.end()) {
            KITA_ENGINE_INFO("[AssetBuilder] StringPath: {} is already stored, returning original", string);
            return it->second;
        }

        auto id = getNextID();
        m_stringPathToID.try_emplace(string, id);
        return id;
    }
} // Kita
