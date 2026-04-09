#pragma once
#include "../Core/DllTemplate.h"
#include "Asset.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <filesystem>
#include "Shader.h"
#include "Texture.h"
#include "../Core/Assert.h"

namespace Kita {
    template <typename T>
    struct AssetBuilder {
        static_assert(std::is_void_v<T>, "Unknown AssetBuilder specialization");
    };

    template <>
    struct AssetBuilder<Shader> {
        template <typename... Args>
        static std::unique_ptr<Shader> build(const std::filesystem::path& path, Args&&... arg) {
            auto shader = Shader::createPtr();
            if (std::expected<void, Shader::ShaderError> result = shader->createShader(std::forward<Args>(arg)...); result.has_value()) {
                return std::move(shader);
            }
            else if (result.error() == Shader::ShaderError::FILE) {
                KITA_ENGINE_ERROR("File error while building shader {}", path.string());
            }
            else if (result.error() == Shader::ShaderError::COMPILATION) {
                KITA_ENGINE_ERROR("Compilation error while building shader {}", path.string());
            }
            else if (result.error() == Shader::ShaderError::LINKING) {
                KITA_ENGINE_ERROR("Linker error while building shader {}", path.string());
            }
            return nullptr;
        }
    };

    template <>
    struct AssetBuilder<Texture> {
        template <typename... Args>
        static std::unique_ptr<Texture> build(const std::filesystem::path& path, Args&&... args) {
            auto texture = Texture::createPtr();
            if (std::expected<void, Texture::TextureError> result = texture->createTexture(std::forward<Args>(args)...); result.has_value()) {
                return std::move(texture);
            }
            else if (result.error() == Texture::TextureError::FILE) {
                KITA_ENGINE_ERROR("File error while building texture {}", path.string());
            }
            return nullptr;
        }
    };


    class KITAENGINE_API AssetManager {
    public:
        using AssetID = uint32_t;

        static inline const std::filesystem::path SHADER_PREFIX = "../assets/shaders/";
        static inline const std::filesystem::path TEXTURE_PREFIX = "../assets/textures/";
        static inline const std::filesystem::path DEFAULT_VERTEX = "DefaultVertex.glsl";
        static inline const std::filesystem::path DEFAULT_FRAGMENT = "DefaultFragment.glsl";
        static inline const std::filesystem::path DEFAULT_TEXTURE = "DefaultTexture.png";

        template <std::derived_from<Asset> T>
        const T& getAsset(const std::filesystem::path& path) const {
            // get unordered_map of correct type
            const auto& bucket = getBucket<T>();
            const std::string key = path.string();

            if (auto it = bucket.find(key); it != bucket.end()) {
                return *it->second;
            }

            // return default if asset for key isn't found
            KITA_ENGINE_ERROR("Asset for key not found, returning default, key: {}", key);
            auto it = bucket.find(defaultID);
            KITA_ENGINE_ASSERT(it != bucket.end(), "Default asset not found");
            return *it->second;
        }

        template <std::derived_from<Asset> T>
        const T& getAsset(const AssetID ID) const {
            // get unordered_map of correct type
            const auto& bucket = getBucket<T>();

            if (auto it = bucket.find(ID); it != bucket.end()) {
                return *it->second;
            }

            // return default if asset for key isn't found
            KITA_ENGINE_ERROR("Asset for ID not found, returning default, ID: {}", ID);
            auto it = bucket.find(defaultID);
            KITA_ENGINE_ASSERT(it != bucket.end(), "Default asset not found");
            return *it->second;
        }

        template <std::derived_from<Asset> T, typename... Args>
        const T& getOrCreateAsset(const std::filesystem::path& path, const bool replace = false, Args&&... args) {
            // get unordered_map of correct type
            auto& bucket = getBucket<T>();
            const std::string pathString = path.string();
            std::optional<AssetID> ID;

            if (replace) {
                ID = getIDForStringPath(pathString);
            }
            else {
                ID = getOrAddStringPath(pathString);
            }

            // try to find the asset first
            if (!replace) {
                if (auto foundIt = bucket.find(ID.value()); foundIt != bucket.end()) {
                    return *foundIt->second;
                }
            }

            // try to build the new asset if it's not present in map
            if (ID.has_value()) {
                if (auto asset = AssetBuilder<T>::build(path, std::forward<Args>(args)...); asset != nullptr) {
                    auto [insertedIt, _] = bucket.insert_or_assign(ID.value(), std::move(asset));
                    return *insertedIt->second;
                }
            }
            else {
                KITA_ENGINE_ERROR("Trying to replace invalid ID for {}", pathString);
            }

            // if the built asset is invalid return the default one
            auto defaultIt = bucket.find(defaultID);
            KITA_ENGINE_ASSERT(defaultIt != bucket.end(), "Default asset not found");
            return *defaultIt->second;
        }

        template <std::derived_from<Asset> T, typename... Args>
        void createAsset(const std::filesystem::path& path, const bool replace = false, const bool setAsDefault = false, Args&&... args) {
            // get unordered_map of correct type
            auto& bucket = getBucket<T>();
            const std::string pathString = path.string();
            std::optional<AssetID> ID;

            if (replace) {
                ID = getIDForStringPath(pathString);
            }
            else {
                ID = getOrAddStringPath(pathString);
            }


            if (auto asset = AssetBuilder<T>::build(path, std::forward<Args>(args)...); asset != nullptr) {
                // replace default asset
                if (setAsDefault) {
                    if (auto [_, ok] = bucket.insert_or_assign(defaultID, std::move(asset)); ok) {
                        KITA_ENGINE_INFO("Set default asset for {}", typeid(T).name());
                    }
                    return;
                }

                if (replace) {
                    if (!ID.has_value()) {
                        KITA_ENGINE_ERROR("Trying to replace invalid ID", pathString);
                        return;
                    }
                    bucket.insert_or_assign(ID.value(), std::move(asset));
                }
                else {
                    auto [_, inserted] = bucket.try_emplace(ID.value(), std::move(asset));

                    if (!inserted) {
                        KITA_ENGINE_ERROR("Asset already exists at {}", pathString);
                    }
                }
            }
        }

    private:
        static constexpr AssetID defaultID = 0;

        friend class Engine;
        static void addDefaultAssets();

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
