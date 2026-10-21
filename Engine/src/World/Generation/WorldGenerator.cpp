#include "WorldGenerator.h"

#include "Noise.h"
#include "../../Core/Engine.h"
#include "../../Renderer/Properties/VertexProperties.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/HeightFieldShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>

namespace Kita {
    WorldGenerator::WorldGenerator(const int octaveCount, const float gain, const float lacunarity) : m_octaveCount(octaveCount), m_gain(gain), m_lacunarity(lacunarity) {
        m_noise = Noise::getTerrainNoise(octaveCount, gain, lacunarity);
    }

    ChunkData WorldGenerator::generateChunkData(const ChunkPos& chunkPos) {
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
                properties.texture = glm::vec2(static_cast<float>(chunkPos.getVertexX(x)), static_cast<float>(chunkPos.getVertexZ(z)));

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

        std::vector<float> physicsHeights;
        physicsHeights.resize(PHYSICS_SAMPLES * PHYSICS_SAMPLES);
        for (int z = 0; z < PHYSICS_SAMPLES; ++z) {
            for (int x = 0; x < PHYSICS_SAMPLES; ++x) {
                const bool padding = x >= VERTICES_PER_EDGE || z >= VERTICES_PER_EDGE;
                physicsHeights[z * PHYSICS_SAMPLES + x] = padding ? JPH::HeightFieldShapeConstants::cNoCollisionValue : heightAt(x, z);
            }
        }

        JPH::HeightFieldShapeSettings heightFieldSettings(physicsHeights.data(), JPH::Vec3::sZero(), JPH::Vec3(1.0f, 1.0f, 1.0f), PHYSICS_SAMPLES);
        JPH::Shape::ShapeResult shapeResult = heightFieldSettings.Create();
        if (shapeResult.HasError()) {
            KITA_ENGINE_DEBUG("[WorldGenerator] Heightfield error: {}", shapeResult.GetError().c_str());
        }

        const JPH::RVec3 bodyPos(static_cast<float>(chunkPos.getVertexX(0)), 0.0f, static_cast<float>(chunkPos.getVertexZ(0)));
        JPH::BodyCreationSettings bodySettings(shapeResult.Get(), bodyPos, JPH::Quat::sIdentity(), JPH::EMotionType::Static, PhysicsLayers::Layers::STATIC);

        ChunkData chunkMeshData(chunkPos, std::move(vertices), std::move(heightmap), std::move(bodySettings));

        return chunkMeshData;
    }

    Chunk WorldGenerator::generateChunk(ChunkData chunkData) {
        const auto chunkPos = chunkData.getChunkPos();
        Chunk chunk(std::move(chunkData));
        KITA_ENGINE_DEBUG("[WorldGenerator] Generated chunk at X:{} Z:{}", chunkPos.getChunkX(), chunkPos.getChunkZ());
        return chunk;
    }
} // Kita
