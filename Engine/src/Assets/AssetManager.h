#pragma once
#include "../Core/DllTemplate.h"
#include "Asset.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <expected>
#include <filesystem>
#include <magic_enum/magic_enum.hpp>
#include "Shader.h"
#include "Texture.h"
#include "../Core/Assert.h"
#include "../Renderer/Scene/Primitives/Mesh.h"

namespace Kita {
    template <typename T>
    struct AssetBuilder {
        static_assert(std::is_void_v<T>, "[AssetBuilder] Unknown AssetBuilder specialization");
    };

    template <>
    struct AssetBuilder<Shader> {
        template <typename... Args>
        static std::unique_ptr<Shader> build(const std::optional<std::filesystem::path>& ignored, Args&&... arg) {
            auto shader = Shader::createPtr();
            if (std::expected<void, Shader::ShaderError> result = shader->createShader(std::forward<Args>(arg)...); result.has_value()) {
                KITA_ENGINE_DEBUG("[AssetBuilder] Created shader asset");
                return std::move(shader);
            }
            else if (result.error().code == Shader::ShaderErrorCode::FILE) {
                KITA_ENGINE_ERROR("[AssetBuilder] File error while building shader {}", result.error().file);
            }
            else if (result.error().code == Shader::ShaderErrorCode::COMPILATION) {
                KITA_ENGINE_ERROR("[AssetBuilder] Compilation error while building shader {}", result.error().file);
            }
            else if (result.error().code == Shader::ShaderErrorCode::LINKING) {
                KITA_ENGINE_ERROR("[AssetBuilder] Linker error while building shader program");
            }
            return nullptr;
        }
    };

    template <typename T>
    concept NotAPath = !std::convertible_to<T, std::filesystem::path>;

    template <>
    struct AssetBuilder<Texture> {
        template <typename... Args>
        static std::unique_ptr<Texture> build(const std::optional<std::filesystem::path>& path, Args&&... args) {
            auto texture = Texture::createPtr();
            if (std::expected<void, Texture::TextureError> result = texture->createTexture(path, std::forward<Args>(args)...); result.has_value()) {
                KITA_ENGINE_DEBUG("[AssetBuilder] Created texture asset with type: {}", magic_enum::enum_name(texture->getType()));
                return std::move(texture);
            }
            else if (result.error() == Texture::TextureError::FILE) {
                KITA_ENGINE_ERROR("[AssetBuilder] File error while building texture {}", path.has_value() ? path.value().string() : "<no path>");
            }
            else if (result.error() == Texture::TextureError::USUPPORTED_NUM_OF_CHANNELS) {
                KITA_ENGINE_ERROR("[AssetBuilder] Unsupported number of channels while building texture {}", path.has_value() ? path.value().string() : "<no path>");
            }
            return nullptr;
        }
    };

    template <>
    struct AssetBuilder<Mesh> {
        template <typename... Args>
        static std::unique_ptr<Mesh> build(const std::optional<std::filesystem::path>& ignored, Args&&... args) {
            KITA_ENGINE_DEBUG("[AssetBuilder] Created mesh asset");
            return std::make_unique<Mesh>(args...);
        }
    };


    class KITAENGINE_API AssetManager {
    public:
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

        template <std::derived_from<Asset> T>
        T& getAsset(const std::filesystem::path& path) const {
            // get unordered_map of correct type
            const auto& bucket = getBucket<T>();
            const std::string key = path.string();

            if (auto it = bucket.find(key); it != bucket.end()) {
                return *it->second;
            }

            // return default if asset for key isn't found
            KITA_ENGINE_ERROR("[AssetBuilder] Asset for key not found, returning default, key: {}", key);
            auto it = bucket.find(DEFAULT_ASSET_ID);
            KITA_ENGINE_ASSERT(it != bucket.end(), "[AssetBuilder] Default asset not found");
            return *it->second;
        }

        template <std::derived_from<Asset> T>
        T& getAsset(const AssetID ID) const {
            // get unordered_map of correct type
            const auto& bucket = getBucket<T>();

            if (auto it = bucket.find(ID); it != bucket.end()) {
                return *it->second;
            }

            // return default if asset for key isn't found
            if (ID == INVALID_ASSET_ID) {
                KITA_ENGINE_ERROR("[AssetBuilder] Asset tried to fetch INVALID_ID, returning default");
            }
            else {
                KITA_ENGINE_ERROR("[AssetBuilder] Asset for ID not found, returning default, ID: {}", ID);
            }
            auto it = bucket.find(DEFAULT_ASSET_ID);
            KITA_ENGINE_ASSERT(it != bucket.end(), "[AssetBuilder] Default asset not found");
            return *it->second;
        }

        template <std::derived_from<Asset> T, typename... Args>
        AssetResult<T> getOrCreateAsset(Args&&... args) {
            return getOrCreateAssetImpl<T>(std::nullopt, false, std::forward<Args>(args)...);
        }

        template <std::derived_from<Asset> T, typename... Args>
        AssetResult<T> getOrCreateAsset(const std::optional<std::filesystem::path>& path, bool replace, Args&&... args) {
            return getOrCreateAssetImpl<T>(path, replace, std::forward<Args>(args)...);
        }

    private:
        template <std::derived_from<Asset> T, typename... Args>
        AssetResult<T> getOrCreateAssetImpl(const std::optional<std::filesystem::path>& path, const bool replace, Args&&... args) {
            // get unordered_map of correct type
            auto& bucket = getBucket<T>();
            std::string pathString;
            std::optional<AssetID> ID;

            if (path.has_value()) {
                pathString = path.value().string();
                ID = replace ? getIDForStringPath(pathString) : getOrAddStringPath(pathString);
            }
            else {
                ID = getNextID();
            }

            // try to find the asset first
            if (!replace) {
                if (auto foundIt = bucket.find(ID.value()); foundIt != bucket.end()) {
                    return AssetResult<T>{.id = ID.value(), .asset = *foundIt->second};
                }
            }

            // try to build the new asset if it's not present in map
            if (ID.has_value()) {
                if (auto asset = buildAsset<T>(path, std::forward<Args>(args)...); asset != nullptr) {
                    auto [insertedIt, _] = bucket.insert_or_assign(ID.value(), std::move(asset));
                    return AssetResult<T>{.id = ID.value(), .asset = *insertedIt->second};
                }
            }
            else {
                KITA_ENGINE_ERROR("[AssetBuilder] Trying to replace invalid ID for {}", pathString);
            }

            // if the built asset is invalid return the default one
            auto defaultIt = bucket.find(DEFAULT_ASSET_ID);
            KITA_ENGINE_ASSERT(defaultIt != bucket.end(), "[AssetBuilder] Default asset not found");
            return AssetResult<T>{.id = DEFAULT_ASSET_ID, .asset = *defaultIt->second};
        }

    public:
        template <std::derived_from<Asset> T, typename... Args>
        AssetID createAsset(Args&&... args) {
            return createAssetImpl<T>(std::nullopt, {}, std::forward<Args>(args)...);
        }

        template <std::derived_from<Asset> T, typename... Args>
        AssetID createAsset(const std::optional<std::filesystem::path>& path, const AssetOptions options, Args&&... args) {
            return createAssetImpl<T>(path, options, std::forward<Args>(args)...);
        }

    private:
        template <std::derived_from<Asset> T, typename... Args>
        AssetID createAssetImpl(const std::optional<std::filesystem::path>& path, const AssetOptions options, Args&&... args) {
            // get unordered_map of correct type
            auto& bucket = getBucket<T>();
            std::string pathString;
            std::optional<AssetID> ID;

            if (path.has_value()) {
                pathString = path.value().string();
                ID = options.replace ? getIDForStringPath(pathString) : getOrAddStringPath(pathString);
            }
            else {
                ID = getNextID();
            }

            if (auto asset = buildAsset<T>(path, std::forward<Args>(args)...); asset != nullptr) {
                // replace default asset
                if (options.setAsDefault) {
                    if (auto [_, ok] = bucket.insert_or_assign(DEFAULT_ASSET_ID, std::move(asset)); ok) {
                        KITA_ENGINE_INFO("[AssetBuilder] Set default asset for {}", typeid(T).name());
                    }
                    return DEFAULT_ASSET_ID;
                }

                if (options.replace) {
                    if (!ID.has_value()) {
                        KITA_ENGINE_ERROR("[AssetBuilder] Trying to replace invalid ID, returning defaultID", pathString);
                        return DEFAULT_ASSET_ID;
                    }
                    bucket.insert_or_assign(ID.value(), std::move(asset));
                    return ID.value();
                }

                //create new
                auto [_, inserted] = bucket.try_emplace(ID.value(), std::move(asset));
                if (!inserted) {
                    KITA_ENGINE_ERROR("[AssetBuilder] Asset already exists at {}, returning existing ID", pathString);
                }
                return ID.value();
            }
            return DEFAULT_ASSET_ID;
        }

    private:
        friend class Engine;

        template <std::derived_from<Asset> T, typename... Args>
        static std::unique_ptr<T> buildAsset(std::optional<std::filesystem::path> path, Args&&... args) {
            return AssetBuilder<T>::build(std::move(path), std::forward<Args>(args)...);
        }

        void addDefaultAssets();

        AssetID getNextID();
        std::optional<AssetID> getIDForStringPath(const std::string& string);
        AssetID getOrAddStringPath(const std::string& string);

        template <std::derived_from<Asset> T>
        static std::unordered_map<AssetID, std::unique_ptr<T>>& getBucket() {
            static std::unordered_map<AssetID, std::unique_ptr<T>> bucket;
            return bucket;
        }

        std::unordered_map<std::string, AssetID> m_stringPathToID;
    };
} // Kita
