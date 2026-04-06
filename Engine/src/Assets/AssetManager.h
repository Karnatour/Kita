#pragma once
#include "../Core/DllTemplate.h"
#include "Asset.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <map>

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
        static std::unique_ptr<Shader> build(Args&&... arg) {
            auto shader = Shader::createPtr();
            return shader->compileShader(std::forward<Args>(arg)...);
        }
    };

    template<>
    struct AssetBuilder<Texture> {
        template <typename... Args>
        static std::unique_ptr<Texture> build(Texture::TextureType type, Args&&... args) {
            auto texture = Texture::createPtr();
            switch (expression) {  }
        }
    };


    class KITAENGINE_API AssetManager {
    public:
        AssetManager(const AssetManager& other) = delete;
        AssetManager& operator=(const AssetManager& other) = delete;

        template <std::derived_from<Asset> T>
        const T& getOrCreateAsset(std::filesystem::path path) {
            return T;
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<Asset>> m_assets;
    };
} // Kita
