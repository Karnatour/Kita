#include "../kitapch.h"

#include "KAsset.h"
#include "../Core/Engine.h"
#include "../Renderer/Scene/ECS/Components/Components.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Kita {
    std::expected<Entity, AssetImporter::ImportError> KAsset::loadFromFile(const std::filesystem::path& path, Scene& scene) {
        std::filesystem::path KAssetPath = BAKED_PREFIX / path;
        KAssetPath.replace_extension("kasset");

        try {
            if (!std::filesystem::exists(KAssetPath)) {
                KITA_ENGINE_INFO(".KAsset file is missing, it will be baked now");
                return std::unexpected(AssetImporter::ImportError::KASSET);
            }

            std::ifstream file(KAssetPath, std::ios::binary | std::ios::in);
            file.exceptions(std::istream::failbit | std::istream::badbit);

            FileHeader fileHeader;
            file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

            checkHeaderFormat(fileHeader, KAssetPath);

            if (fileHeader.sourceFileHash != FileReader::getFileHash(AssetImporter::MODELS_PREFIX / path)) {
                return std::unexpected(AssetImporter::ImportError::HASH_MISMATCH);
            }

            Entity rootEntity;

            std::unordered_map<uint32_t, Entity> entities;

            for (uint32_t i = 0; i < fileHeader.nodeCount; ++i) {
                NodeHeader nodeHeader;
                file.read(reinterpret_cast<char*>(&nodeHeader), sizeof(nodeHeader));

                Entity nodeEntity = scene.createEntity();
                nodeEntity.addComponent<ChildrenComponent>();
                entities.insert({nodeHeader.nodeIndex, nodeEntity});
                if (i == 0) {
                    rootEntity = nodeEntity;
                } else {
                    entities.at(nodeHeader.parentIndex).getComponent<ChildrenComponent>().children.push_back(nodeEntity.getEnttEntityID());
                }

                for (uint32_t j = 0; j < nodeHeader.subNodesCount; ++j) {
                    SubNodeHeader subNodeHeader;
                    file.read(reinterpret_cast<char*>(&subNodeHeader), sizeof(subNodeHeader));

                    Entity chunkEntity = scene.createEntity();
                    chunkEntity.addComponent<RenderInShadowPass>();
                    chunkEntity.addComponent<RenderInMainPass>();
                    for (int k = 0; k < subNodeHeader.chunkCount; ++k) {
                        ChunkHeader chunkHeader;
                        file.read(reinterpret_cast<char*>(&chunkHeader), sizeof(chunkHeader));
                        loadChunk(file, chunkHeader, chunkEntity);
                    }
                    nodeEntity.getComponent<ChildrenComponent>().children.push_back(chunkEntity.getEnttEntityID());
                }
            }

            rootEntity.addComponent<PhysicsComponent>(PhysicsComponent{.bodyID = Engine::getEngine()->getPhysicsManager().createBody(rootEntity, JPH::EMotionType::Static, PhysicsLayers::STATIC, JPH::EActivation::Activate)});

            return rootEntity;
        }
        catch (const std::ifstream::failure& e) {
            KITA_ENGINE_ERROR("Error while loading KAsset. Path {}, error: {}", KAssetPath.string(), e.what());
            return std::unexpected(AssetImporter::ImportError::KASSET);
        }
        catch (const std::runtime_error& e) {
            KITA_ENGINE_ERROR("Error while loading KAsset. Path {}, error: {}", KAssetPath.string(), e.what());
            return std::unexpected(AssetImporter::ImportError::KASSET);
        }
    }

    bool KAsset::saveToFile(const Entity rootEntity, const std::filesystem::path& path) {
        const std::filesystem::path KAssetPath = KAsset::BAKED_PREFIX / std::filesystem::path(path).replace_extension("kasset");

        try {
            std::filesystem::create_directories(KAssetPath.parent_path());

            std::ofstream file(KAssetPath, std::ios::binary | std::ios::trunc);
            file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

            const uint32_t nodeCount = calculateNodeCount(rootEntity);
            writeFileHeader(file, nodeCount, AssetImporter::MODELS_PREFIX / path);

            uint32_t nodeIndex = 0;
            writeNodes(file, rootEntity, nodeIndex, 0);

            KITA_ENGINE_DEBUG("[KAsset] Finished saving KAsset: {} ", KAssetPath.string());
            return true;
        }
        catch (const std::ofstream::failure& e) {
            KITA_ENGINE_ERROR("[KAsset] Error while saving KAsset. Path {}, error: {}", KAssetPath.string(), e.what());
            return false;
        }
        catch (const std::runtime_error& e) {
            KITA_ENGINE_ERROR("[KAsset] Error while saving KAsset. Path {}, error: {}", KAssetPath.string(), e.what());
            return false;
        }
    }

    uint32_t KAsset::calculateNodeCount(Entity entity) {
        uint32_t nodeCount = 0;

        if (entity.hasAllComponents<ChildrenComponent>()) {
            nodeCount = nodeCount + 1;
            for (const auto child : entity.getComponent<ChildrenComponent>().children) {
                nodeCount += calculateNodeCount(Entity(entity.getScene(), child));
            }
        }
        return nodeCount;
    }

    void KAsset::loadChunk(std::ifstream& file, ChunkHeader chunk, Entity chunkEntity) {
        const auto chunkStart = file.tellg();

        switch (static_cast<ChunkType>(chunk.chunkType)) {
            case ChunkType::MESH: {
                chunkEntity.addComponent<MeshComponent>(readMesh(file));
                break;
            }
            case ChunkType::MATERIAL: {
                chunkEntity.addComponent<MaterialComponent>(readMaterial(file));
                break;
            }
            case ChunkType::TRANSFORMATION:
                chunkEntity.addComponent<TransformationComponent>(readTransformation(file));
                break;
        }

        if (const auto chunkEnd = file.tellg(); static_cast<uint64_t>(chunkEnd - chunkStart) != chunk.chunkSize) {
            throw std::runtime_error("Chunk size mismatch! Expected " + std::to_string(chunk.chunkSize) + ", read " + std::to_string(chunkEnd - chunkStart));
        }
    }

    void KAsset::checkHeaderFormat(const FileHeader& fileHeader, const std::filesystem::path& path) {
        if (fileHeader.magicNumber != MAGIC_NUMBER) {
            throw std::runtime_error("Invalid magic number in asset file: " + path.string());
        }
        if (fileHeader.versionFormat != VERSION_FORMAT) {
            throw std::runtime_error("Unsupported version in asset file: " + path.string());
        }
    }

    MeshComponent KAsset::readMesh(std::ifstream& file) {
        MeshHeader meshHeader;
        file.read(reinterpret_cast<char*>(&meshHeader), sizeof(meshHeader));

        std::vector<VertexProperties> vertices;
        vertices.resize(meshHeader.vertexCount);
        file.read(reinterpret_cast<char*>(vertices.data()), static_cast<std::streamsize>(vertices.size() * sizeof(VertexProperties)));

        std::vector<unsigned int> indices;
        indices.resize(meshHeader.indexCount);
        file.read(reinterpret_cast<char*>(indices.data()), static_cast<std::streamsize>(indices.size() * sizeof(unsigned int)));

        return MeshComponent{Engine::getEngine()->getAssetManager().createAsset<Mesh>(std::move(vertices), std::move(indices))};
    }

    MaterialComponent KAsset::readMaterial(std::ifstream& file) {
        MaterialData materialHeader{};
        file.read(reinterpret_cast<char*>(&materialHeader), sizeof(materialHeader));

        MaterialComponent materialComponent;

        for (int j = 0; j < materialHeader.textureCount; ++j) {
            const auto textureType = static_cast<Texture::TextureType>(materialHeader.textureTypes[j]);
            auto [textureID, textureAsset] = Engine::getEngine()->getAssetManager().getOrCreateAsset<Texture>(materialHeader.texturePaths[j], {}, textureType, std::nullopt);
            switch (textureType) {
                case Texture::TextureType::ALBEDO:
                    materialComponent.albedoTextureID = textureID;
                    break;
                case Texture::TextureType::METALLIC_ROUGHNESS:
                    materialComponent.metallicRoughnessTextureID = textureID;
                    break;
                case Texture::TextureType::NORMAL:
                    materialComponent.normalTextureID = textureID;
                    break;
                default:
                    KITA_ENGINE_WARN("[KAsset] Unknown texture type while reading material");
            }
        }
        auto [shaderID, shaderAsset] = Engine::getEngine()->getAssetManager().getOrCreateAsset<Shader>(std::string(materialHeader.shaderPaths[0]) + std::string(materialHeader.shaderPaths[1]), {},
                                                                                                       std::initializer_list{Shader::vert(materialHeader.shaderPaths[0]), Shader::frag(materialHeader.shaderPaths[1])});

        materialComponent.shaderID = shaderID;

        return materialComponent;
    }

    TransformationComponent KAsset::readTransformation(std::ifstream& file) {
        TransformationData transformationData;
        file.read(reinterpret_cast<char*>(&transformationData), sizeof(transformationData));

        const glm::vec3 translation(transformationData.translation[0], transformationData.translation[1], transformationData.translation[2]);
        const glm::quat rotation(transformationData.rotation[3], transformationData.rotation[0], transformationData.rotation[1], transformationData.rotation[2]);
        const glm::vec3 scale(transformationData.scale[0], transformationData.scale[1], transformationData.scale[2]);

        return TransformationComponent{.worldModel = glm::recompose(scale, rotation, translation, glm::vec3(0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))};
    }


    void KAsset::writeNodes(std::ofstream& file, Entity entity, uint32_t& nodeIndex, uint32_t parentIndex) {
        NodeHeader nodeHeader;
        nodeHeader.parentIndex = parentIndex;
        nodeHeader.nodeIndex = nodeIndex;

        //Name
        if (entity.hasAllComponents<NameComponent>()) {
            const std::string& name = entity.getComponent<NameComponent>().name;

            if (name.length() >= MAX_NAME_LENGTH) {
                KITA_ENGINE_WARN("Node name length: {} is same or larger than max allowed name length: {}", name.length(), MAX_NAME_LENGTH);
            }

            const size_t n = std::min(name.size(), sizeof(nodeHeader.name) - 1);
            std::copy_n(name.begin(), n, nodeHeader.name);
            nodeHeader.name[n] = '\0';
        }

        //SubNodesCount
        for (const auto child : entity.getComponent<ChildrenComponent>().children) {
            auto childEntity = Entity(entity.getScene(), child);
            if (childEntity.hasAnyComponent<MeshComponent, MaterialComponent, TransformationComponent>()) {
                nodeHeader.subNodesCount = nodeHeader.subNodesCount + 1;
            }
        }

        file.write(reinterpret_cast<const char*>(&nodeHeader), sizeof(nodeHeader));

        for (const auto child : entity.getComponent<ChildrenComponent>().children) {
            auto childEntity = Entity(entity.getScene(), child);

            if (!childEntity.hasAnyComponent<MeshComponent, MaterialComponent, TransformationComponent>()) {
                continue;
            }

            uint32_t subChunkCount = 0;
            if (childEntity.hasAllComponents<MeshComponent>()) {
                subChunkCount = subChunkCount + 1;
            }
            if (childEntity.hasAllComponents<MaterialComponent>()) {
                subChunkCount = subChunkCount + 1;
            }
            if (childEntity.hasAllComponents<TransformationComponent>()) {
                subChunkCount = subChunkCount + 1;
            }

            SubNodeHeader subNodeHeader;
            if (childEntity.hasAllComponents<NameComponent>()) {
                const std::string name = childEntity.getComponent<NameComponent>().name;
                const size_t n = std::min(name.size(), sizeof(subNodeHeader.name) - 1);
                std::copy_n(name.begin(), n, subNodeHeader.name);
                subNodeHeader.name[n] = '\0';
            }
            subNodeHeader.chunkCount = subChunkCount;
            file.write(reinterpret_cast<const char*>(&subNodeHeader), sizeof(subNodeHeader));

            if (childEntity.hasAllComponents<MeshComponent>()) {
                writeMesh(file, childEntity);
            }
            if (childEntity.hasAllComponents<MaterialComponent>()) {
                writeMaterial(file, childEntity);
            }
            if (childEntity.hasAllComponents<TransformationComponent>()) {
                writeTransformation(file, childEntity);
            }
        }

        for (const auto child : entity.getComponent<ChildrenComponent>().children) {
            auto childEntity = Entity(entity.getScene(), child);
            // only entity that has ChildrenComponent is Node
            if (childEntity.hasAllComponents<ChildrenComponent>()) {
                nodeIndex = nodeIndex + 1;
                writeNodes(file, childEntity, nodeIndex, nodeHeader.nodeIndex);
            }
        }
    }

    void KAsset::writeFileHeader(std::ofstream& file, const uint32_t nodeCount, const std::filesystem::path& filePath) {
        FileHeader fileHeader;
        const std::optional<uint64_t> fileHash = FileReader::getFileHash(filePath);
        if (!fileHash) {
            throw std::runtime_error(fmt::format("Unable to calculate file hash for: {}", filePath.string()));
        }
        fileHeader.sourceFileHash = fileHash.value();
        fileHeader.nodeCount = nodeCount;
        file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    }

    void KAsset::writeMesh(std::ofstream& file, Entity entity) {
        if (entity.hasAllComponents<MeshComponent>()) {
            auto [meshID] = entity.getComponent<MeshComponent>();

            const Mesh& mesh = Engine::getEngine()->getAssetManager().getAsset<Mesh>(meshID);

            VertexBuffer& vbo = mesh.getVertexBuffer();
            IndexBuffer* ibo = mesh.getIndexBuffer();

            const auto& vertices = vbo.getVertices();
            const auto* indices = ibo != nullptr ? &ibo->getIndices() : nullptr;

            ChunkHeader chunk;
            chunk.chunkType = static_cast<uint32_t>(ChunkType::MESH);
            if (indices != nullptr) {
                chunk.chunkSize = sizeof(MeshHeader) + sizeof(VertexProperties) * vertices.size() + sizeof(unsigned int) * indices->size();
            } else {
                chunk.chunkSize = sizeof(MeshHeader) + sizeof(VertexProperties) * vertices.size();
            }
            file.write(reinterpret_cast<const char*>(&chunk), sizeof(chunk));

            MeshHeader geometryHeader;
            geometryHeader.vertexCount = static_cast<uint32_t>(vertices.size());
            geometryHeader.indexCount = indices != nullptr ? static_cast<uint32_t>(indices->size()) : 0;

            file.write(reinterpret_cast<const char*>(&geometryHeader), sizeof(geometryHeader));
            file.write(reinterpret_cast<const char*>(vertices.data()), static_cast<std::streamsize>(vertices.size() * sizeof(VertexProperties)));
            if (indices != nullptr) {
                file.write(reinterpret_cast<const char*>(indices->data()), static_cast<std::streamsize>(indices->size() * sizeof(unsigned int)));
            }
        }
    }

    void KAsset::writeMaterial(std::ofstream& file, Entity entity) {
        if (entity.hasAllComponents<MaterialComponent>()) {
            MaterialData materialData{};

            std::array<Texture*, 3> textures;

            const auto& [shaderID, albedoTextureID, metallicRoughnessTextureID, normalTextureID] = entity.getComponent<MaterialComponent>();
            auto& assetManager = Engine::getEngine()->getAssetManager();
            if (albedoTextureID != AssetManager::INVALID_ASSET_ID) {
                textures[materialData.textureCount++] = &assetManager.getAsset<Texture>(albedoTextureID);
            }
            if (metallicRoughnessTextureID != AssetManager::INVALID_ASSET_ID) {
                textures[materialData.textureCount++] = &assetManager.getAsset<Texture>(metallicRoughnessTextureID);
            }
            if (normalTextureID != AssetManager::INVALID_ASSET_ID) {
                textures[materialData.textureCount++] = &assetManager.getAsset<Texture>(normalTextureID);
            }

            writeTextures(materialData, textures);

            writeShader(materialData, assetManager.getAsset<Shader>(shaderID));

            ChunkHeader chunk;
            chunk.chunkType = static_cast<uint32_t>(ChunkType::MATERIAL);
            chunk.chunkSize = sizeof(MaterialData);
            file.write(reinterpret_cast<const char*>(&chunk), sizeof(chunk));

            file.write(reinterpret_cast<const char*>(&materialData), sizeof(materialData));
        }
    }

    void KAsset::writeTextures(MaterialData& materialHeader, const std::span<Texture* const> textures) {
        if (materialHeader.textureCount > MAX_MATERIAL_TEXTURE_PATH_COUNT) {
            throw std::runtime_error(fmt::format("Texture count: {} is larger than max allowed texture count: {}", materialHeader.textureCount, MAX_MATERIAL_TEXTURE_PATH_COUNT));
        }

        for (int i = 0; i < materialHeader.textureCount; i++) {
            if (textures[i]->getPath()->string().length() >= MAX_PATH_LENGTH) {
                throw std::runtime_error(fmt::format("TexturePath is larger than: {} causing overflow: {}", MAX_PATH_LENGTH, textures[i]->getPath()->string()));
            }
            strcpy_s(materialHeader.texturePaths[i], textures[i]->getPath()->string().c_str());
            materialHeader.textureTypes[i] = static_cast<unsigned char>(textures[i]->getType());
        }
    }

    void KAsset::writeShader(MaterialData& materialHeader, const Shader& shader) {
        auto& shadersInfo = shader.getShadersInfo();

        std::string vertexPath;
        std::string fragmentPath;

        for (auto shaderInfo : shadersInfo) {
            if (shaderInfo.type == Shader::ShaderType::VERTEX) {
                vertexPath = shaderInfo.path.string();
            }
            if (shaderInfo.type == Shader::ShaderType::FRAGMENT) {
                fragmentPath = shaderInfo.path.string();
            }
        }

        if (vertexPath.empty()) {
            throw std::runtime_error("VertexShaderPath is empty");
        }
        if (fragmentPath.empty()) {
            throw std::runtime_error("FragmentShaderPath is empty");
        }

        //Vertex
        if (vertexPath.length() >= MAX_PATH_LENGTH) {
            throw std::runtime_error(fmt::format("VertexShaderPath is larger than: {} causing overflow: {}", MAX_PATH_LENGTH, vertexPath));
        }
        strcpy_s(materialHeader.shaderPaths[0], vertexPath.c_str());

        //Fragment
        if (fragmentPath.length() >= MAX_PATH_LENGTH) {
            throw std::runtime_error(fmt::format("FragmentShaderPath is larger than: {} causing overflow: {}", MAX_PATH_LENGTH, fragmentPath));
        }
        strcpy_s(materialHeader.shaderPaths[1], fragmentPath.c_str());
    }

    void KAsset::writeTransformation(std::ofstream& file, Entity entity) {
        if (entity.hasAllComponents<TransformationComponent>()) {
            glm::vec3 scale, translation, skew;
            glm::quat rotation;
            glm::vec4 perspective;

            glm::decompose(entity.getComponent<TransformationComponent>().worldModel, scale, rotation, translation, skew, perspective);

            TransformationData transformationData;
            transformationData.translation[0] = translation.x;
            transformationData.translation[1] = translation.y;
            transformationData.translation[2] = translation.z;

            transformationData.scale[0] = scale.x;
            transformationData.scale[1] = scale.y;
            transformationData.scale[2] = scale.z;

            transformationData.rotation[0] = rotation.x;
            transformationData.rotation[1] = rotation.y;
            transformationData.rotation[2] = rotation.z;
            transformationData.rotation[3] = rotation.w;

            ChunkHeader chunk;
            chunk.chunkType = static_cast<uint32_t>(ChunkType::TRANSFORMATION);
            chunk.chunkSize = sizeof(TransformationData);
            file.write(reinterpret_cast<const char*>(&chunk), sizeof(chunk));
            file.write(reinterpret_cast<const char*>(&transformationData), sizeof(transformationData));
        }
    }
} // Kita
