#pragma once

#include <assimp/scene.h>

#include "AssetManager.h"
#include "Texture.h"
#include "../Core/DllTemplate.h"
#include "../Renderer/Properties/VertexProperties.h"
#include "../Renderer/Scene/ECS/Entity.h"

namespace Kita {
    class KITAENGINE_API AssetImporter {
    public:
        enum class ImportError {
            FILE
        };

        static inline const std::filesystem::path MODELS_PREFIX = "../assets/models";
        static std::expected<Entity, ImportError> importModel(const std::filesystem::path& path, Scene& scene, bool reimport = false);

    private:
        struct Material {
            AssetManager::AssetID albedoTextureID = AssetManager::INVALID_ASSET_ID;
            AssetManager::AssetID metallicRoughnessTextureID = AssetManager::INVALID_ASSET_ID;
            AssetManager::AssetID normalTextureID = AssetManager::INVALID_ASSET_ID;

            //TODO Proper blending
            bool ignoreBecauseOfAlpha = false;
        };

        static std::vector<Material> importMaterials(const aiScene* aiScene, const std::filesystem::path& path);
        static void processNode(const aiScene* aiScene, const aiNode* aiNode, Scene& scene, Entity parentEntity, const std::vector<Material>& materials);
        static void addMaterialComponents(Entity entity, const aiMesh* aiMesh, const std::vector<Material>& materials);
        static glm::mat4 convertAiModelMatrixToGLM(const aiMatrix4x4& aiMatrix);
        static Entity processMesh(const aiMesh* aiMesh, Scene& scene, const std::vector<Material>& materials, const aiMatrix4x4& aiTransformMatrix);
        static VertexProperties importVertex(const aiMesh& aiMesh, unsigned int index);
        static Texture::TextureType assimpToKitaTextureType(const aiTextureType& ai_texture);
        static std::optional<AssetManager::AssetID> importTexture(aiTextureType textureType, const aiMaterial& aiMaterial, const std::filesystem::path& path);
        static void moveTexture(const std::filesystem::path& texturePath);
    };
} // Kita
