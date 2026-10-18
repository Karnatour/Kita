#include "ChunkData.h"

#include "WorldGenerator.h"
#include "../../Renderer/Properties/VertexProperties.h"

namespace Kita {
    ChunkData::ChunkData(const ChunkPos& chunkPos, std::vector<VertexProperties> vertices, std::array<float, HEIGHTMAP_SIZE> heightmap, JPH::BodyCreationSettings bodyCreationSettings)
        : m_chunkPos(chunkPos), m_vertices(std::move(vertices)), m_heightmap(std::move(heightmap)), m_bodySettings(std::move(bodyCreationSettings)) {
    }

    ChunkPos ChunkData::getChunkPos() const {
        return m_chunkPos;
    }

    const std::vector<VertexProperties>& ChunkData::getVertices() const {
        return m_vertices;
    }

    const JPH::BodyCreationSettings& ChunkData::getBodyCreationSettings() const {
        return m_bodySettings;
    }

    const std::array<float, ChunkData::HEIGHTMAP_SIZE>& ChunkData::getHeightmap() const {
        return m_heightmap;
    }

    void ChunkData::setBodyID(const JPH::BodyID bodyID) {
        m_bodyID = bodyID;
    }

    JPH::BodyID ChunkData::getBodyID() const {
        return m_bodyID;
    }
} // Kita
