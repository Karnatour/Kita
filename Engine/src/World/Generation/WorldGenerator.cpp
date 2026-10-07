#include "WorldGenerator.h"

#include "Noise.h"
#include "../../Renderer/Properties/VertexProperties.h"

namespace Kita {
    WorldGenerator::WorldGenerator(const int octaveCount, const int gain, const int lacunarity) : m_octaveCount(octaveCount), m_gain(gain), m_lacunarity(lacunarity) {
        m_noise = Noise::getTerrainNoise(octaveCount, gain, lacunarity);
    }

    Chunk WorldGenerator::generateChunk(const ChunkPos& chunkPos) {
        constexpr int QUADS_PER_EDGE = Chunk::CHUNK_SIZE;
        constexpr int VERTICES_PER_EDGE = QUADS_PER_EDGE + 1; // +1 -> One more vertex than quads because N quads need N+1 vertices
        constexpr int SAMPLES_PER_EDGE = VERTICES_PER_EDGE + 2; //  +2 -> Normals need neighbours, so we need one extra sample on each side

        constexpr float HEIGHT_SCALE = 100.0f; // Used for converting noise values (-1,1) to meters
        constexpr float NOISE_FREQUENCY = 0.01f; // Lower = bigger hills
        constexpr int SEED = 69420; // TODO move

        std::array<float, SAMPLES_PER_EDGE * SAMPLES_PER_EDGE> heightmap;
        m_noise->GenUniformGrid2D(heightmap.data(),
                                  chunkPos.getVertexX(0) - 1, chunkPos.getVertexZ(0) - 1, //X and Y Offset -1 so we have border data also
                                  SAMPLES_PER_EDGE, SAMPLES_PER_EDGE, NOISE_FREQUENCY, NOISE_FREQUENCY, SEED);


        // This returns height value in meters for local vertex position. Due to border we shift by +1 so center is correct
        auto heightAt = [&](const int x, const int z) {
            return heightmap[(x + 1) * SAMPLES_PER_EDGE + (z + 1)] * HEIGHT_SCALE;
        };

        std::vector<VertexProperties> vertices;
        vertices.reserve(VERTICES_PER_EDGE * VERTICES_PER_EDGE);

        for (int z = 0; z < VERTICES_PER_EDGE; ++z) {
            for (int x = 0; x < VERTICES_PER_EDGE; ++x) {
                VertexProperties properties;
                properties.position = glm::vec3(static_cast<float>(chunkPos.getVertexX(x)), heightAt(x, z), static_cast<float>(chunkPos.getVertexZ(z)));
                properties.normal = glm::normalize(glm::vec3(
                    heightAt(x - 1, z) - heightAt(x + 1, z),
                    2.0f, // We sample one left and one right neighbour -> 2.0f;
                    heightAt(x, z - 1) - heightAt(x, z + 1)));
                vertices.emplace_back(std::move(properties));
            }
        }

        std::vector<unsigned int> indices;
        indices.reserve(QUADS_PER_EDGE * QUADS_PER_EDGE);
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
        Chunk chunk(chunkPos);

        return chunk;
    }
} // Kita
