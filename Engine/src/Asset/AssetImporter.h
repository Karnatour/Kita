#pragma once

#include <assimp/scene.h>

#include "../Core/DllTemplate.h"
#include "../Renderer/Scene/Primitives/Model.h"

namespace Kita {
    class KITAENGINE_API AssetImporter {
    public:
        static inline const std::filesystem::path MODELS_PREFIX = "../assets/models";
        static std::shared_ptr<Model> importModel(const std::filesystem::path& path, bool reimport = false);

    private:
        static void processNode(const aiScene* aiScene, const aiNode* aiNode, const std::shared_ptr<Model>& model);
        static Vertex importVertex(const aiMesh& aiMesh, unsigned int index);
        static Texture::TextureType assimpToKitaTextureType(const aiTextureType& ai_texture);
        static void importTextures(aiTextureType textureType, const aiMaterial& aiMaterial, const std::shared_ptr<Material>& material, const std::filesystem::path& path);
        static void moveTexture(const std::filesystem::path& texturePath);
        static void importPhongProperies(const aiMaterial& aiMaterial, const std::shared_ptr<Material>& material);
    };
} // Kita
