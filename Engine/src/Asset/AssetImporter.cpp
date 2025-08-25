#include "../kitapch.h"
#include "AssetImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "KAsset.h"
#include "../Core/Engine.h"

namespace Kita {
    std::shared_ptr<Model> AssetImporter::importModel(const std::filesystem::path& path, const bool reimport) {
        std::filesystem::path kassetPath = path;
        kassetPath.replace_extension("kasset");
        if (!reimport && KAsset::alreadyBaked(kassetPath)) {
            KITA_ENGINE_DEBUG("Asset already baked, loading .kasset {}", kassetPath.string());
            return KAsset::loadFromFile(KAsset::BAKED_PREFIX / kassetPath);
        }

        const std::filesystem::path filePath(MODELS_PREFIX / path);

        Assimp::Importer importer;
        const aiScene* aiScene = importer.ReadFile(
            filePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices |
            aiProcess_ImproveCacheLocality | aiProcess_SortByPType | aiProcess_OptimizeMeshes | aiProcess_ValidateDataStructure);

        if (!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode) {
            KITA_ENGINE_ERROR("Assimp error while reading file, returning nullptr error: {}", importer.GetErrorString());
            return nullptr;
        }
        auto model = std::make_shared<Model>();

        for (int materialIndex = 0; materialIndex < aiScene->mNumMaterials; ++materialIndex) {
            const aiMaterial* aiMaterial = aiScene->mMaterials[materialIndex];
            auto material = std::make_shared<Material>();

            importTextures(aiTextureType_DIFFUSE, *aiMaterial, material, path);
            importTextures(aiTextureType_SPECULAR, *aiMaterial, material, path);

            importPhongProperies(*aiMaterial, material->getPhongProperties());

            model->addMaterial(material);
        }

        processNode(aiScene, aiScene->mRootNode, model);

        KAsset::saveToFile(model, KAsset::BAKED_PREFIX / std::filesystem::path(path).replace_extension("kasset"));
        model->setPath(path);

        return model;
    }

    void AssetImporter::processNode(const aiScene* aiScene, const aiNode* aiNode, const std::shared_ptr<Model>& model) {
        if (aiNode == nullptr) {
            return;
        }

        for (unsigned int m = 0; m < aiNode->mNumMeshes; m++) {
            const aiMesh* aiMesh = aiScene->mMeshes[aiNode->mMeshes[m]];

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            vertices.reserve(aiMesh->mNumVertices);
            for (unsigned int v = 0; v < aiMesh->mNumVertices; v++) {
                vertices.push_back(importVertex(*aiMesh, v));
            }

            for (unsigned int f = 0; f < aiMesh->mNumFaces; f++) {
                const aiFace& aiFace = aiMesh->mFaces[f];
                indices.insert(indices.end(), aiFace.mIndices, aiFace.mIndices + aiFace.mNumIndices);
            }
            model->addMesh(std::make_shared<Mesh>(vertices, indices, aiMesh->mMaterialIndex));
        }

        for (unsigned int i = 0; i < aiNode->mNumChildren; i++) {
            processNode(aiScene, aiNode->mChildren[i], model);
        }
    }

    Vertex AssetImporter::importVertex(const aiMesh& aiMesh, const unsigned int index) {
        Vertex vertex{};
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
        return vertex;
    }

    void AssetImporter::importTextures(const aiTextureType textureType, const aiMaterial& aiMaterial, const std::shared_ptr<Material>& material, const std::filesystem::path& path) {
        for (unsigned int i = 0; i < aiMaterial.GetTextureCount(textureType); ++i) {
            aiString aiStr;
            aiMaterial.GetTexture(textureType, i, &aiStr);
            std::filesystem::path texturePath = path;
            texturePath.replace_filename(aiStr.C_Str());
            moveTexture(texturePath);
            auto& textureManager = Engine::getEngine()->getRenderer().getTextureManager();
            textureManager.addTexture(texturePath);
            material->addTexture(textureManager.getTexture(texturePath.string()));
        }
    }

    void AssetImporter::moveTexture(const std::filesystem::path& texturePath) {
        if (std::filesystem::exists(MODELS_PREFIX / texturePath)) {
            std::filesystem::create_directories(TextureManager::TEXTURE_PREFIX / texturePath.parent_path());
            std::filesystem::rename(MODELS_PREFIX / texturePath, TextureManager::TEXTURE_PREFIX / texturePath);
        }
    }

    void AssetImporter::importPhongProperies(const aiMaterial& aiMaterial, PhongProperties& phongProperties) {
        aiColor3D aiColor;
        if (AI_SUCCESS == aiMaterial.Get(AI_MATKEY_COLOR_DIFFUSE, aiColor)) {
            phongProperties.diffuse = {aiColor.r, aiColor.g, aiColor.b};
        }

        if (AI_SUCCESS == aiMaterial.Get(AI_MATKEY_COLOR_AMBIENT, aiColor)) {
            phongProperties.ambient = {aiColor.r, aiColor.g, aiColor.b};
        }

        if (AI_SUCCESS == aiMaterial.Get(AI_MATKEY_COLOR_SPECULAR, aiColor)) {
            phongProperties.specular = {aiColor.r, aiColor.g, aiColor.b};
        }

        float shininnes;
        if (AI_SUCCESS == aiMaterial.Get(AI_MATKEY_SHININESS, shininnes)) {
            phongProperties.shininess = shininnes;
        }
    }
} // Kita
