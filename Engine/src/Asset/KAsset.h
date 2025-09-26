#pragma once

#include <cstdint>

#include "../Core/DllTemplate.h"
#include "../Renderer/Scene/Primitives/Model.h"

namespace Kita {
    class KITAENGINE_API KAsset {
    public:
        static inline const std::filesystem::path BAKED_PREFIX = "../assets/baked";
        static constexpr uint32_t MAGIC_NUMBER = 0x4b495441; // KITA
        static constexpr uint32_t VERSION_FORMAT = 100;

        enum class ChunkType : uint32_t {
            MESH = 0x4D455348, // MESH
            MATERIAL = 0x4D415445, // MATE
            NONE = 0x4E4F4E45 // NONE
        };

        struct FileHeader {
            uint32_t magicNumber = MAGIC_NUMBER;
            uint32_t versionFormat = VERSION_FORMAT;
            uint32_t chunkCount = 0;
            uint64_t totalSize = 0;
        };

        struct ChunkHeader {
            uint32_t chunkType = static_cast<uint32_t>(ChunkType::NONE);
            uint64_t chunkSize = 0;
        };

        struct Node {
            char name[64];
            int32_t parentIndex; // -1 = root
            float translation[3];
            float rotation[4];
            float scale[3];
            int32_t meshIndex; // -1 = no mesh
        };

        struct MeshHeader {
            uint32_t vertexCount = 0;
            uint32_t indexCount = 0;
            uint32_t materialIndex = -1; // -1 = no material
        };

        struct MaterialHeader {
            uint32_t textureCount;
            float ambient[3];
            float diffuse[3];
            float specular[3];
            float shininess;
            char texturePaths[8][128];
            unsigned char textureTypes[64];
            char shaderPaths[2][64]; //vertex, fragment
        };

        static std::shared_ptr<Model> loadFromFile(const std::filesystem::path& path);
        static bool saveToFile(const std::shared_ptr<Model>& model, const std::filesystem::path& path);
        static void fetchExistingBakedFiles();
        static bool alreadyBaked(const std::filesystem::path& path);
        static inline std::unordered_set<std::filesystem::path> m_bakedFiles;

    private:
        static void loadChunk(std::ifstream& file, ChunkHeader& chunk, const std::shared_ptr<Model>& model);
        static void cheackHeaderFormat(const FileHeader& header, const std::filesystem::path& path);
        static std::shared_ptr<Mesh> readMesh(std::ifstream& file);
        static std::shared_ptr<Material> readMaterial(std::ifstream& file);
        static void writeHeaderStart(std::ofstream& file, FileHeader& header, uint32_t chunkCount);
        static void writeHeaderEnd(std::ofstream& file, const FileHeader& header);
        static void writeMeshes(std::ofstream& file, const std::vector<std::shared_ptr<Mesh>>& meshes);
        static void writeMaterials(std::ofstream& file, const std::vector<std::shared_ptr<Material>>& materials);
        static void writeTextures(MaterialHeader& materialHeader, const std::vector<std::shared_ptr<Texture>>& textures);
        static void writeShader(MaterialHeader& materialHeader, const std::shared_ptr<Shader>& shader);

        static void vec3ToFloat(const glm::vec3& vec, float (&arr)[3]);
        static void FloatToVec3(glm::vec4& vec, const float (&arr)[3]);
    };
} // Kita
