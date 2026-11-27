#include "WorldGenerator.h"

#include "Noise.h"
#include "../../Core/Engine.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/HeightFieldShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>

namespace Kita {
    WorldGenerator::WorldGenerator(const int octaveCount, const float gain, const float lacunarity) : m_octaveCount(octaveCount), m_gain(gain), m_lacunarity(lacunarity) {
        m_noise = Noise::getTerrainNoise(octaveCount, gain, lacunarity);
    }

    ChunkData WorldGenerator::generateChunkData(const NodeKey& key) {
        std::array<float, SAMPLES_PER_EDGE * SAMPLES_PER_EDGE> heightmap;
        const ChunkPos chunkPos(key.chunkPosX, key.chunkPosZ);
        const int lod = key.lodLevel;
        const float step = static_cast<float>(1 << lod);
        const float nodeSize = static_cast<float>(ChunkData::CHUNK_SIZE) * step;

        const float originX = static_cast<float>(key.chunkPosX) * nodeSize;
        const float originZ = static_cast<float>(key.chunkPosZ) * nodeSize;

        // -step so we have border data also
        const float startX = originX - step;
        const float startZ = originZ - step;
        m_noise->GenUniformGrid2D(heightmap.data(), startX, startZ, SAMPLES_PER_EDGE, SAMPLES_PER_EDGE, step, step, SEED);

        for (float& height : heightmap) {
            height = height * HEIGHT_SCALE;
        }

        // This returns height value in meters for local vertex position. Due to border we shift by +1 so center is correct
        auto heightAt = [&](const int x, const int z) {
            return heightmap[(z + 1) * SAMPLES_PER_EDGE + (x + 1)];
        };

        std::vector<std::byte> vertices;
        vertices.reserve(VERTICES_PER_EDGE * VERTICES_PER_EDGE);

        auto minMaxY = std::make_pair(0.0f, 0.0f);
        auto& [minY,maxY] = minMaxY;
        for (int z = 0; z < VERTICES_PER_EDGE; ++z) {
            for (int x = 0; x < VERTICES_PER_EDGE; ++x) {
                float y = heightAt(x, z);
                minY = std::min(minY, y);
                maxY = std::max(maxY, y);

                // Position
                VertexBufferData::appendBytes(vertices, glm::vec3(originX + static_cast<float>(x) * step, y, originZ + static_cast<float>(z) * step));

                const float heightLeft = heightAt(x - 1, z);
                const float heightRight = heightAt(x + 1, z);
                const float heightBack = heightAt(x, z - 1);
                const float heightFront = heightAt(x, z + 1);

                const float dYdX = (heightRight - heightLeft) / (2.0f * step);
                const float dYdZ = (heightFront - heightBack) / (2.0f * step);

                // Slope
                VertexBufferData::appendBytes(vertices, glm::vec2(dYdX, dYdZ));
            }
        }

        std::optional<JPH::BodyCreationSettings> bodySettings;
        if (lod == 0) {
            std::vector<float> physicsHeights(PHYSICS_SAMPLES * PHYSICS_SAMPLES);
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

            const JPH::RVec3 bodyPos(originX, 0.0f, originZ);
            bodySettings.emplace(shapeResult.Get(), bodyPos, JPH::Quat::sIdentity(), JPH::EMotionType::Static, PhysicsLayers::Layers::STATIC);
        }

        return ChunkData(chunkPos, std::move(vertices), std::move(heightmap), std::move(bodySettings), minMaxY);
    }

    Chunk WorldGenerator::generateChunk(ChunkData chunkData) {
        const auto chunkPos = chunkData.getChunkPos();
        Chunk chunk(std::move(chunkData));
        KITA_ENGINE_DEBUG("[WorldGenerator] Generated chunk at X:{} Z:{}", chunkPos.getChunkX(), chunkPos.getChunkZ());
        return chunk;
    }
} // Kita
