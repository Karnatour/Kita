#include "WorldGenerator.h"

#include "Noise.h"
#include "../../Core/Engine.h"
#include "../../Renderer/Properties/VertexProperties.h"

namespace Kita {
    WorldGenerator::WorldGenerator(const int octaveCount, const float gain, const float lacunarity) : m_octaveCount(octaveCount), m_gain(gain), m_lacunarity(lacunarity) {
        m_noise = Noise::getTerrainNoise(octaveCount, gain, lacunarity);
    }

    ChunkMeshData WorldGenerator::generateChunkMeshData(const ChunkPos& chunkPos) {
        constexpr int QUADS_PER_EDGE = Chunk::CHUNK_SIZE;
        constexpr int VERTICES_PER_EDGE = QUADS_PER_EDGE + 1; // +1 -> One more vertex than quads because N quads need N+1 vertices
        constexpr int SAMPLES_PER_EDGE = VERTICES_PER_EDGE + 2; //  +2 -> Normals need neighbours, so we need one extra sample on each side

        constexpr float HEIGHT_SCALE = 250.0f; // Used for converting noise values (-1,1) to meters
        constexpr float NOISE_FREQUENCY = 1.0f / 64.0f; // Lower = bigger hills. This needs to be power of two otherwise will cause white dots
        constexpr int SEED = 69420; // TODO move

        std::array<float, SAMPLES_PER_EDGE * SAMPLES_PER_EDGE> heightmap;
        //X and Y Offset -1 so we have border data also
        const float startX = static_cast<float>(chunkPos.getVertexX(0) - 1) * NOISE_FREQUENCY;
        const float startZ = static_cast<float>(chunkPos.getVertexZ(0) - 1) * NOISE_FREQUENCY;
        m_noise->GenUniformGrid2D(heightmap.data(), startX, startZ, SAMPLES_PER_EDGE, SAMPLES_PER_EDGE, NOISE_FREQUENCY, NOISE_FREQUENCY, SEED);

        // This returns height value in meters for local vertex position. Due to border we shift by +1 so center is correct
        auto heightAt = [&](const int x, const int z) {
            return heightmap[(z + 1) * SAMPLES_PER_EDGE + (x + 1)] * HEIGHT_SCALE;
        };

        std::vector<VertexProperties> vertices;
        vertices.reserve(VERTICES_PER_EDGE * VERTICES_PER_EDGE);

        for (int z = 0; z < VERTICES_PER_EDGE; ++z) {
            for (int x = 0; x < VERTICES_PER_EDGE; ++x) {
                VertexProperties properties;
                properties.position = glm::vec3(static_cast<float>(chunkPos.getVertexX(x)), heightAt(x, z), static_cast<float>(chunkPos.getVertexZ(z)));
                properties.texture = glm::vec2(static_cast<float>(chunkPos.getVertexX(x)), static_cast<float>(chunkPos.getVertexZ(z))) / 4.0f;

                const float heightLeft = heightAt(x - 1, z);
                const float heightRight = heightAt(x + 1, z);
                const float heightBack = heightAt(x, z - 1);
                const float heightFront = heightAt(x, z + 1);

                const float dYdX = (heightRight - heightLeft) * 0.5f;
                const float dYdZ = (heightFront - heightBack) * 0.5f;

                properties.normal = glm::normalize(glm::vec3(-dYdX, 1.0f, -dYdZ));
                properties.tangent = glm::normalize(glm::vec3(1.0f, dYdX, 0.0f));
                properties.bitangent = glm::cross(properties.tangent, properties.normal);

                vertices.emplace_back(std::move(properties));
            }
        }

        std::vector<unsigned int> indices;
        indices.reserve(QUADS_PER_EDGE * QUADS_PER_EDGE * 6);
        for (int z = 0; z < QUADS_PER_EDGE; ++z) {
            for (int x = 0; x < QUADS_PER_EDGE; ++x) {
                const unsigned int topLeft = z * VERTICES_PER_EDGE + x;
                const unsigned int topRight = topLeft + 1;
                const unsigned int bottomLeft = topLeft + VERTICES_PER_EDGE;
                const unsigned int bottomRight = bottomLeft + 1;

                // Triangle left
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                // Triangle right
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        ChunkMeshData chunkMeshData(chunkPos, std::move(vertices), std::move(indices));
        return chunkMeshData;
    }

    Chunk WorldGenerator::generateChunk(ChunkMeshData chunkMeshData) {
        const auto chunkPos = chunkMeshData.getChunkPos();
        Chunk chunk(chunkPos, Engine::getEngine()->getAssetManager().createAsset<Mesh>(std::move(chunkMeshData.getVertices()), std::move(chunkMeshData.getIndices())));
        KITA_ENGINE_DEBUG("[WorldGenerator] Generated chunk at X:{} Z:{}", chunkPos.getChunkX(), chunkPos.getChunkZ());
        return chunk;
    }
} // Kita
