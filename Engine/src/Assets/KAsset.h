#pragma once

#include <unordered_set>

#include "AssetImporter.h"
#include "../Core/DllTemplate.h"
#include "../Renderer/Scene/ECS/Components/MaterialComponent.h"
#include "../Renderer/Scene/ECS/Components/MeshComponent.h"
#include "../Renderer/Scene/ECS/Components/TransformationComponent.h"

namespace Kita {
    class KITAENGINE_API KAsset {
    public:
        static inline const std::filesystem::path BAKED_PREFIX = "../assets/baked";
        static constexpr uint32_t MAGIC_NUMBER = 0x4b495441; // KITA
        static constexpr uint32_t VERSION_FORMAT = 100;

        static constexpr int MAX_MATERIAL_TEXTURE_PATH_COUNT = 3;
        static constexpr int MAX_MATERIAL_SHADER_PATH_COUNT = 2;
        static constexpr int MAX_NAME_LENGTH = 64;
        static constexpr int MAX_PATH_LENGTH = 512;

        enum class ChunkType : uint32_t {
            MESH = 0x4D455348, // MESH
            MATERIAL = 0x4D415445, // MATE
            TRANSFORMATION = 0x5452414E // TRAN
        };

        struct FileHeader {
            uint32_t magicNumber = MAGIC_NUMBER;
            uint32_t versionFormat = VERSION_FORMAT;
            uint64_t sourceFileHash;
            uint32_t nodeCount = 0;
        };

        struct NodeHeader {
            uint32_t parentIndex = std::numeric_limits<unsigned int>().max();
            uint32_t nodeIndex = std::numeric_limits<unsigned int>().max();
            uint32_t subNodesCount = 0;
            char name[MAX_NAME_LENGTH] = "Unnamed node";
        };

        struct SubNodeHeader {
            char name[MAX_NAME_LENGTH] = "Unnamed mesh";
            uint32_t chunkCount = 0;
        };

        struct ChunkHeader {
            uint32_t chunkType;
            uint64_t chunkSize = 0;
        };

        struct MeshHeader {
            uint32_t vertexCount = 0;
            uint32_t indexCount = 0;
        };

        struct MaterialData {
            int32_t textureCount;
            unsigned char textureTypes[MAX_MATERIAL_TEXTURE_PATH_COUNT];
            char texturePaths[MAX_MATERIAL_TEXTURE_PATH_COUNT][MAX_PATH_LENGTH];
            char shaderPaths[MAX_MATERIAL_SHADER_PATH_COUNT][MAX_PATH_LENGTH]; //vertex, fragment
        };

        struct TransformationData {
            float translation[3] = {0.0f, 0.0f, 0.0f};
            float rotation[4] = {0.0f, 0.0f, 0.0f, 1.0f};
            float scale[3] = {1.0f, 1.0f, 1.0f};
        };

        static std::expected<Entity, AssetImporter::ImportError> loadFromFile(const std::filesystem::path& path, Scene& scene);
        static bool saveToFile(Entity rootEntity, const std::filesystem::path& path);

    private:
        static uint32_t calculateNodeCount(Entity entity);
        static void loadChunk(std::ifstream& file, ChunkHeader chunk, Entity chunkEntity);
        static void checkHeaderFormat(const FileHeader& fileHeader, const std::filesystem::path& path);
        static MeshComponent readMesh(std::ifstream& file);
        static MaterialComponent readMaterial(std::ifstream& file);
        static TransformationComponent readTransformation(std::ifstream& file);
        static void writeNodes(std::ofstream& file, Entity entity, uint32_t& nodeIndex, uint32_t parentIndex);
        static void writeFileHeader(std::ofstream& file, uint32_t nodeCount, const std::filesystem::path& filePath);
        static void writeMesh(std::ofstream& file, Entity entity);
        static void writeMaterial(std::ofstream& file, Entity entity);
        static void writeTextures(MaterialData& materialHeader, std::span<Texture* const> textures);
        static void writeShader(MaterialData& materialHeader, const Shader& shader);
        static void writeTransformation(std::ofstream& file, Entity entity);


    };
} // Kita
