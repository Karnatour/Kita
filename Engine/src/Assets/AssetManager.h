#pragma once
#include "../Core/DllTemplate.h"
#include "Asset.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <expected>
#include <filesystem>
#include <typeindex>
#include <magic_enum/magic_enum.hpp>
#include "Shader.h"
#include "Texture.h"
#include "../Core/Assert.h"
#include "Mesh.h"
#include "AssetKey.h"

namespace Kita {
    template <typename T>
    struct AssetBuilder {
        static_assert(std::is_void_v<T>, "[AssetBuilder] Unknown AssetBuilder specialization");
    };

    //region AssetBuilding
    template <>
    struct AssetBuilder<Shader> {
        template <typename... Args>
        static std::unique_ptr<Shader> build(AssetKey key, Args&&... arg) {
            auto shader = Shader::createPtr();
            if (std::expected<void, Shader::ShaderError> result = shader->createShader(std::move(key).takePaths(), std::forward<Args>(arg)...); result.has_value()) {
                KITA_ENGINE_INFO("[AssetBuilder] Created shader asset");
                return shader;
            } else if (result.error().code == Shader::ShaderErrorCode::FILE) {
                KITA_ENGINE_ERROR("[AssetBuilder] File error while building shader {}", result.error().file);
            } else if (result.error().code == Shader::ShaderErrorCode::COMPILATION) {
                KITA_ENGINE_ERROR("[AssetBuilder] Compilation error while building shader {}", result.error().file);
            } else if (result.error().code == Shader::ShaderErrorCode::LINKING) {
                KITA_ENGINE_ERROR("[AssetBuilder] Linker error while building shader program");
            } else if (result.error().code == Shader::ShaderErrorCode::SIZE_MISMATCH) {
                KITA_ENGINE_ERROR("[AssetBuilder] ShaderPath-ShaderInfo size mismatch");
            } else if (result.error().code == Shader::ShaderErrorCode::REPLACE) {
                KITA_ENGINE_ERROR("[AssetBuilder] Error while replacing defines in shader {}", result.error().file);
            }
            return nullptr;
        }
    };

    template <typename T>
    concept NotAPath = !std::convertible_to<T, std::filesystem::path>;

    template <>
    struct AssetBuilder<Texture> {
        template <typename... Args>
        static std::unique_ptr<Texture> build(AssetKey key, Args&&... args) {
            auto texture = Texture::createPtr();
            if (std::expected<void, Texture::TextureError> result = texture->createTexture(std::move(key).takePaths(), std::forward<Args>(args)...); result.has_value()) {
                KITA_ENGINE_INFO("[AssetBuilder] Created texture asset with type: {}", magic_enum::enum_name(texture->getType()));
                return texture;
            } else if (result.error().code == Texture::TextureErrorCode::FILE) {
                KITA_ENGINE_ERROR("[AssetBuilder] File error while building texture {}", result.error().errorStr);
            } else if (result.error().code == Texture::TextureErrorCode::UNSUPPORTED_TEXTURE_ARRAY_FOR_TYPE) {
                KITA_ENGINE_ERROR("[AssetBuilder] Texture array is not supported this type, texture {}", result.error().errorStr);
            } else if (result.error().code == Texture::TextureErrorCode::UNSUPPORTED_NUM_OF_CHANNELS) {
                KITA_ENGINE_ERROR("[AssetBuilder] Unsupported number of channels while building texture {}", result.error().errorStr);
            } else if (result.error().code == Texture::TextureErrorCode::PROPERTIES_MISMATCH) {
                KITA_ENGINE_ERROR("[AssetBuilder] Properties mismatch while building texture {}", result.error().errorStr);
            } else if (result.error().code == Texture::TextureErrorCode::EMPTY) {
                KITA_ENGINE_ERROR("[AssetBuilder] Received empty list of paths for this buffer type: {}", result.error().errorStr);
            }
            return nullptr;
        }
    };

    template <>
    struct AssetBuilder<Mesh> {
        template <typename... Args>
        static std::unique_ptr<Mesh> build(AssetKey /*ignored*/, Args&&... args) {
            KITA_ENGINE_DEBUG("[AssetBuilder] Created mesh asset");
            return std::make_unique<Mesh>(std::forward<Args>(args)...);
        }
    };

    // endregion

    class KITAENGINE_API AssetManager {
    public:
        AssetManager() = default;
        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;
        AssetManager(AssetManager&&) = default;
        AssetManager& operator=(AssetManager&&) = default;

        using AssetID = uint32_t;
        static constexpr AssetID DEFAULT_ASSET_ID = 0;
        static constexpr AssetID INVALID_ASSET_ID = std::numeric_limits<uint32_t>::max();

        static inline const std::filesystem::path SHADER_PREFIX = "../assets/shaders/";
        static inline const std::filesystem::path TEXTURE_PREFIX = "../assets/textures/";
        static inline const std::filesystem::path DEFAULT_VERTEX = "KitaDefaultVertex.glsl";
        static inline const std::filesystem::path DEFAULT_FRAGMENT = "KitaDefaultFragment.glsl";
        static inline const std::filesystem::path DEFAULT_TEXTURE = "DefaultTexture.png";

        struct AssetOptions {
            bool replace = false;
            bool setAsDefault = false;
        };

        template <std::derived_from<Asset> T>
        struct AssetResult {
            AssetID id;
            T& asset;
        };

        //region Getters
        template <std::derived_from<Asset> T>
        T& getAsset(const std::filesystem::path& path) const {
            // get unordered_map of correct type
            const auto& bucket = getBucket<T>();
            const auto it = m_stringPathToID.find(path.string());

            if (it != m_stringPathToID.end()) {
                return getAsset<T>(it->second);
            }

            // return default if asset for key isn't found
            KITA_ENGINE_ERROR("[AssetBuilder] Asset for key not found, returning default, path: {}", path.string());
            auto defaultIt = bucket.find(DEFAULT_ASSET_ID);
            KITA_ENGINE_ASSERT(defaultIt != bucket.end(), "[AssetBuilder] Default asset not found");
            return static_cast<T&>(*defaultIt->second);
        }

        template <std::derived_from<Asset> T>
        T& getAsset(const AssetID ID) const {
            // get unordered_map of correct type
            const auto& bucket = getBucket<T>();

            if (auto it = bucket.find(ID); it != bucket.end()) {
                return static_cast<T&>(*it->second);
            }

            // return default if asset for key isn't found
            if (ID == INVALID_ASSET_ID) {
                KITA_ENGINE_ERROR("[AssetBuilder] Asset tried to fetch INVALID_ID, returning default");
            } else {
                KITA_ENGINE_ERROR("[AssetBuilder] Asset for ID not found, returning default, ID: {}", ID);
            }
            auto it = bucket.find(DEFAULT_ASSET_ID);
            KITA_ENGINE_ASSERT(it != bucket.end(), "[AssetBuilder] Default asset not found");
            return static_cast<T&>(*it->second);
        }

        template <std::derived_from<Asset> T>
        size_t getTotalNumberOfAssetsForBucket() const {
            // get unordered_map of correct type
            const auto& bucket = getBucket<T>();

            return bucket.size();
        }

        //endregion

        template <std::derived_from<Asset> T, typename... Args>
        AssetResult<T> getOrCreateAsset(AssetKey key, const AssetOptions options, Args&&... args) {
            return getOrCreateAssetImpl<T>(std::move(key), options, std::forward<Args>(args)...);
        }

    private:
        template <std::derived_from<Asset> T>
        AssetResult<T> getDefaultAsset(auto& bucket) {
            auto defaultIt = bucket.find(DEFAULT_ASSET_ID);
            KITA_ENGINE_ASSERT(defaultIt != bucket.end(), "[AssetBuilder] Default asset not found");
            return AssetResult<T>{.id = DEFAULT_ASSET_ID, .asset = static_cast<T&>(*defaultIt->second)};
        }

        template <std::derived_from<Asset> T, typename... Args>
        AssetResult<T> getOrCreateAssetImpl(AssetKey key, const AssetOptions options, Args&&... args) {
            // get unordered_map of correct type
            auto& bucket = getBucket<T>();
            const std::string pathString = key.toString();

            const std::optional<AssetID> ID = getAssetID(options, pathString);
            if (!ID.has_value()) {
                KITA_ENGINE_ERROR("[AssetBuilder] Unable to get asset ID, paths: {}", key.getPathsCombined());
                return getDefaultAsset<T>(bucket);
            }

            // Try to find the asset first, skip finding if replace is true
            if (!options.replace) {
                if (auto foundIt = bucket.find(ID.value()); foundIt != bucket.end()) {
                    return AssetResult<T>{.id = ID.value(), .asset = static_cast<T&>(*foundIt->second)};
                }
            }

            // Try to build the new asset if it's not present in map
            auto asset = buildAsset<T>(std::move(key), std::forward<Args>(args)...);
            if (asset == nullptr) {
                return getDefaultAsset<T>(bucket);
            }

            // Insert_or_assign will insert the asset if it doesn't exist.
            // And replace if it already exists (We are okay with replace since we return early if we dont want to replace)
            auto [it, _] = bucket.insert_or_assign(ID.value(), std::move(asset));
            return AssetResult<T>{.id = ID.value(), .asset = static_cast<T&>(*it->second)};
        }

        template <std::derived_from<Asset> T, typename... Args>
        static std::unique_ptr<T> buildAsset(AssetKey key, Args&&... args) {
            return AssetBuilder<T>::build(std::move(key), std::forward<Args>(args)...);
        }

        template <std::derived_from<Asset> T>
        std::unordered_map<AssetID, std::unique_ptr<Asset>>& getBucket() const {
            return m_buckets[std::type_index(typeid(T))];
        }

        mutable std::unordered_map<std::type_index, std::unordered_map<AssetID, std::unique_ptr<Asset>>> m_buckets;

        friend class Engine;
        void addDefaultAssets();

        std::optional<AssetID> getAssetID(AssetOptions options, const std::string& pathString);
        AssetID getNextID();
        std::optional<AssetID> getIDForStringPath(const std::string& string);
        AssetID getOrAddStringPath(const std::string& string);

        std::unordered_map<std::string, AssetID> m_stringPathToID;
    };
} // Kita
