#include "ChunkData.h"

#include "WorldGenerator.h"

namespace Kita {
    ChunkData::ChunkData(const ChunkPos& chunkPos, std::vector<std::byte> vertices, std::array<float, HEIGHTMAP_SIZE> heightmap, std::optional<JPH::BodyCreationSettings> bodyCreationSettings, const std::pair<float, float> minMaxY)
        : m_chunkPos(chunkPos), m_vertices(std::move(vertices)), m_heightmap(std::move(heightmap)), m_bodySettings(std::move(bodyCreationSettings)), m_minMaxY(minMaxY) {
    }

    ChunkPos ChunkData::getChunkPos() const {
        return m_chunkPos;
    }

    const std::vector<std::byte>& ChunkData::getVertices() const {
        return m_vertices;
    }

    const std::optional<JPH::BodyCreationSettings>& ChunkData::getBodyCreationSettings() const {
        return m_bodySettings;
    }

    const std::array<float, ChunkData::HEIGHTMAP_SIZE>& ChunkData::getHeightmap() const {
        return m_heightmap;
    }

    std::pair<float, float> ChunkData::getMinMaxY() const {
        return m_minMaxY;
    }

    void ChunkData::setBodyID(const JPH::BodyID bodyID) {
        m_bodyID = bodyID;
    }

    JPH::BodyID ChunkData::getBodyID() const {
        return m_bodyID;
    }
} // Kita
