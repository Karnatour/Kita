#include "ChunkData.h"

#include "WorldGenerator.h"
#include "../../Renderer/Properties/VertexProperties.h"

namespace Kita {
    ChunkData::ChunkData(const ChunkPos& chunkPos, std::vector<VertexProperties> vertices, std::array<float, HEIGHTMAP_SIZE> heightmap, JPH::ShapeRefC shape)
        : m_chunkPos(chunkPos), m_vertices(std::move(vertices)), m_heightmap(std::move(heightmap)), m_collisionShape(std::move(shape)) {
    }

    ChunkPos ChunkData::getChunkPos() {
        return m_chunkPos;
    }

    const std::vector<VertexProperties>& ChunkData::getVertices() const {
        return m_vertices;
    }

    const JPH::ShapeRefC& ChunkData::getCollisionShape() const {
        return m_collisionShape;
    }

    const std::array<float, ChunkData::HEIGHTMAP_SIZE>& ChunkData::getHeightmap() const {
        return m_heightmap;
    }
} // Kita
