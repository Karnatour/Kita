#pragma once
#include <vector>
#include <array>

#include "../ChunkPos.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>
#include "../../Renderer/Properties/VertexProperties.h"

namespace Kita {
    class ChunkData {
    public:
        static constexpr int CHUNK_SIZE = 16;
        static constexpr int CHUNK_QUAD_PER_EDGE = 16;
        static constexpr int HEIGHTMAP_EDGE = CHUNK_QUAD_PER_EDGE + 3;
        static constexpr int HEIGHTMAP_SIZE = HEIGHTMAP_EDGE * HEIGHTMAP_EDGE;

        ChunkData(const ChunkPos& chunkPos, std::vector<VertexProperties> vertices, std::array<float, HEIGHTMAP_SIZE> heightmap, JPH::ShapeRefC shape);

        ChunkPos getChunkPos();
        const std::vector<VertexProperties>& getVertices() const;
        const JPH::ShapeRefC& getCollisionShape() const;
        const std::array<float, HEIGHTMAP_SIZE>& getHeightmap() const;

    private:
        ChunkPos m_chunkPos;
        std::vector<VertexProperties> m_vertices;
        std::array<float, HEIGHTMAP_SIZE> m_heightmap;
        JPH::ShapeRefC m_collisionShape;
    };
} // Kita
