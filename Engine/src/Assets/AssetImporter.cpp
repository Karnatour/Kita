#include "../kitapch.h"
#include "AssetImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "KAsset.h"
#include "../Core/Engine.h"
#include "../Renderer/Scene/ECS/Components/MeshComponent.h"
#include "../Renderer/Scene/ECS/Components/PathComponent.h"
#include "../Renderer/Scene/ECS/Components/RelationshipComponents.h"
#include "../Renderer/Scene/ECS/Components/RenderTags.h"
#include "../Renderer/Scene/ECS/Components/TransformationComponent.h"

namespace Kita {
    std::expected<Entity, AssetImporter::ImportError> AssetImporter::importModel(const std::filesystem::path& path, Scene& scene, const bool reimport) {
        /*std::filesystem::path kassetPath = path;
        kassetPath.replace_extension("kasset");
        if (!reimport && KAsset::alreadyBaked(kassetPath)) {
            KITA_ENGINE_DEBUG("Asset already baked, loading .kasset {}", kassetPath.string());
            return KAsset::loadFromFile(KAsset::BAKED_PREFIX / kassetPath);
        }*/

        const std::filesystem::path filePath(MODELS_PREFIX / path);

        Assimp::Importer importer;
        const aiScene* aiScene = importer.ReadFile(
            filePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices |
            aiProcess_ImproveCacheLocality | aiProcess_SortByPType | aiProcess_OptimizeMeshes | aiProcess_ValidateDataStructure | aiProcess_PreTransformVertices |
            aiProcess_GlobalScale);

        if (!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode) {
            return std::unexpected(ImportError::FILE);
        }

        auto rootEntity = scene.createEntity();

        auto materials = importMaterials(aiScene, path);

        processNode(aiScene, aiScene->mRootNode, scene, rootEntity, materials);

        //KAsset::saveToFile(model, KAsset::BAKED_PREFIX / std::filesystem::path(path).replace_extension("kasset"));
        rootEntity.addComponent<PathComponent>(path);

        return rootEntity;
    }

    std::vector<AssetImporter::Material> AssetImporter::importMaterials(const aiScene* aiScene, const std::filesystem::path& path) {
        std::vector<Material> materials;
        materials.resize(aiScene->mNumMaterials);

        for (int materialIndex = 0; materialIndex < aiScene->mNumMaterials; ++materialIndex) {
            const aiMaterial* aiMaterial = aiScene->mMaterials[materialIndex];

            auto& [diffuseComponent, specularComponent, normalComponent, phongProperties] = materials[materialIndex];
            diffuseComponent = importTexture(aiTextureType_DIFFUSE, *aiMaterial, path);
            specularComponent = importTexture(aiTextureType_SPECULAR, *aiMaterial, path);
            normalComponent = importTexture(aiTextureType_NORMALS, *aiMaterial, path);

            phongProperties = importPhongProperies(*aiMaterial);
        }

        return materials;
    }

    void AssetImporter::processNode(const aiScene* aiScene, const aiNode* aiNode, Scene& scene, Entity parentEntity, const std::vector<Material>& materials) {
        if (aiNode == nullptr || (aiNode->mNumMeshes == 0 && aiNode->mChildren == nullptr)) {
            return;
        }

        parentEntity.addComponent<ChildrenComponent>();

        // we must fetch the component before every addition since entt can reallocate and invalidate the reference
        for (unsigned int m = 0; m < aiNode->mNumMeshes; m++) {
            const aiMesh* aiMesh = aiScene->mMeshes[aiNode->mMeshes[m]];
            parentEntity.getComponent<ChildrenComponent>().children.emplace_back(processMesh(aiMesh, scene, materials, aiNode->mTransformation).getEnttEntityID());
        }

        for (unsigned int i = 0; i < aiNode->mNumChildren; i++) {
            const Entity newNodeEntity = scene.createEntity();
            parentEntity.getComponent<ChildrenComponent>().children.emplace_back(newNodeEntity.getEnttEntityID());
            processNode(aiScene, aiNode->mChildren[i], scene, newNodeEntity, materials);
        }
    }

    Entity AssetImporter::processMesh(const aiMesh* aiMesh, Scene& scene, const std::vector<Material>& materials, const aiMatrix4x4& aiTransformMatrix) {
        Entity newEntity = scene.createEntity();

        std::vector<VertexProperties> vertices;
        std::vector<unsigned int> indices;

        vertices.reserve(aiMesh->mNumVertices);
        for (unsigned int v = 0; v < aiMesh->mNumVertices; v++) {
            vertices.emplace_back(importVertex(*aiMesh, v));
        }

        for (unsigned int f = 0; f < aiMesh->mNumFaces; f++) {
            const aiFace& aiFace = aiMesh->mFaces[f];
            indices.insert(indices.end(), aiFace.mIndices, aiFace.mIndices + aiFace.mNumIndices);
        }

        addMaterialComponents(newEntity, aiMesh, materials);
        newEntity.addComponent<MeshComponent>(Engine::getEngine()->getAssetManager().createAsset<Mesh>(vertices, indices));
        newEntity.addComponent<RenderInMainPass>();
        newEntity.addComponent<ShaderComponent>();
        newEntity.addComponent<TransformationComponent>(TransformationComponent{.model = convertAiModelMatrixToGLM(aiTransformMatrix)});
        return newEntity;
    }

    void AssetImporter::addMaterialComponents(Entity entity, const aiMesh* aiMesh, const std::vector<Material>& materials) {
        if (aiMesh->mMaterialIndex > materials.size()) {
            KITA_ENGINE_ERROR("Invalid material index for mesh: {}", aiMesh->mName.C_Str());
            return;
        }

        const auto& [diffuseComponent, specularComponent, normalComponent, phongProperties] = materials[aiMesh->mMaterialIndex];

        if (diffuseComponent.has_value()) {
            entity.addComponent<DiffuseMapComponent>(diffuseComponent.value());
        }
        if (specularComponent.has_value()) {
            entity.addComponent<SpecularMapComponent>(specularComponent.value());
        }
        if (normalComponent.has_value()) {
            entity.addComponent<NormalMapComponent>(normalComponent.value());
        }

        entity.addComponent<PhongProperties>(phongProperties);
    }

    glm::mat4 AssetImporter::convertAiModelMatrixToGLM(const aiMatrix4x4& aiMatrix) {
        return {
            aiMatrix.a1, aiMatrix.a2, aiMatrix.a3, aiMatrix.a4,
            aiMatrix.b1, aiMatrix.b2, aiMatrix.b3, aiMatrix.b4,
            aiMatrix.c1, aiMatrix.c2, aiMatrix.c3, aiMatrix.c4,
            aiMatrix.d1, aiMatrix.d2, aiMatrix.d3, aiMatrix.d4
        };
    }

    VertexProperties AssetImporter::importVertex(const aiMesh& aiMesh, const unsigned int index) {
        VertexProperties vertex{};
        vertex.position.x = aiMesh.mVertices[index].x;
        vertex.position.y = aiMesh.mVertices[index].y;
        vertex.position.z = aiMesh.mVertices[index].z;

        if (aiMesh.HasTextureCoords(0)) {
            vertex.texture.s = aiMesh.mTextureCoords[0][index].x;
            vertex.texture.t = aiMesh.mTextureCoords[0][index].y;
        }

        if (aiMesh.HasVertexColors(0)) {
            vertex.color.r = aiMesh.mColors[0][index].r;
            vertex.color.g = aiMesh.mColors[0][index].g;
            vertex.color.b = aiMesh.mColors[0][index].b;
            vertex.color.a = aiMesh.mColors[0][index].a;
        }
        if (aiMesh.HasNormals()) {
            vertex.normal.x = aiMesh.mNormals[index].x;
            vertex.normal.y = aiMesh.mNormals[index].y;
            vertex.normal.z = aiMesh.mNormals[index].z;
        }
        return vertex;
    }

    Texture::TextureType AssetImporter::assimpToKitaTextureType(const aiTextureType& ai_texture) {
        switch (ai_texture) {
            case aiTextureType_DIFFUSE:
                return Texture::TextureType::DIFFUSE;
            case aiTextureType_SPECULAR:
                return Texture::TextureType::SPECULAR;
            case aiTextureType_NORMALS:
                return Texture::TextureType::NORMAL;
            default:
                return Texture::TextureType::NONE;
        }
    }

    std::optional<AssetManager::AssetID> AssetImporter::importTexture(const aiTextureType textureType, const aiMaterial& aiMaterial, const std::filesystem::path& path) {
        if (aiMaterial.GetTextureCount(textureType) == 0) {
            return std::nullopt;
        }

        if (aiMaterial.GetTextureCount(textureType) > 1) {
            KITA_ENGINE_WARN("Material {} has more than one texture for texture type {}", path.string(), magic_enum::enum_name(assimpToKitaTextureType(textureType)));
        }

        aiString aiStr;
        aiMaterial.GetTexture(textureType, 0, &aiStr);

        std::filesystem::path texturePath = path;
        texturePath.replace_filename(aiStr.C_Str());

        //Move texture to AssetManager texture folder
        moveTexture(texturePath);

        return Engine::getEngine()->getAssetManager().createAsset<Texture>(texturePath, {}, assimpToKitaTextureType(textureType), std::nullopt);
    }

    void AssetImporter::moveTexture(const std::filesystem::path& texturePath) {
        if (std::filesystem::exists(MODELS_PREFIX / texturePath)) {
            std::filesystem::create_directories(AssetManager::TEXTURE_PREFIX / texturePath.parent_path());
            std::filesystem::rename(MODELS_PREFIX / texturePath, AssetManager::TEXTURE_PREFIX / texturePath);
        }
    }

    PhongProperties AssetImporter::importPhongProperies(const aiMaterial& aiMaterial) {
        aiColor3D aiColor;
        PhongProperties phongProperties;
        if (AI_SUCCESS == aiMaterial.Get(AI_MATKEY_COLOR_AMBIENT, aiColor)) {
            phongProperties.ambient = glm::vec3(aiColor.r, aiColor.g, aiColor.b);
        }

        if (AI_SUCCESS == aiMaterial.Get(AI_MATKEY_COLOR_DIFFUSE, aiColor)) {
            phongProperties.diffuse = glm::vec3(aiColor.r, aiColor.g, aiColor.b);
        }

        if (AI_SUCCESS == aiMaterial.Get(AI_MATKEY_COLOR_SPECULAR, aiColor)) {
            phongProperties.specular = glm::vec3(aiColor.r, aiColor.g, aiColor.b);
        }

        if (float shininnes; AI_SUCCESS == aiMaterial.Get(AI_MATKEY_SHININESS, shininnes)) {
            phongProperties.shininess = shininnes;
        }

        if (phongProperties.ambient == glm::vec3(0.0f)) {
            phongProperties.ambient = glm::vec3(0.3f, 0.3f, 0.3f);
        }

        return phongProperties;
    }
} // Kita
