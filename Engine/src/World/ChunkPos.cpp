#include "ChunkPos.h"

#include "Chunk.h"

namespace Kita {
    ChunkPos::ChunkPos(const std::pair<int, int> chunkPos) : m_chunkPos(chunkPos) {
    }

    ChunkPos::ChunkPos(int x, int z) : m_chunkPos(std::make_pair(x, z)) {
    }

    int ChunkPos::getChunkX() const {
        return m_chunkPos.first;
    }

    int ChunkPos::getChunkZ() const {
        return m_chunkPos.second;
    }

    int ChunkPos::getVertexX(const int localX) const {
        return m_chunkPos.first * ChunkData::CHUNK_SIZE + localX;
    }

    int ChunkPos::getVertexZ(const int localZ) const {
        return m_chunkPos.second * ChunkData::CHUNK_SIZE + localZ;
    }

    std::pair<int, int> ChunkPos::getChunkPos() const {
        return m_chunkPos;
    }

    bool ChunkPos::isChunkInRadius(const ChunkPos& middleChunkPos, const int renderDistance) const {
        return std::abs(m_chunkPos.first - middleChunkPos.getChunkX()) <= renderDistance && std::abs(m_chunkPos.second - middleChunkPos.getChunkZ()) <= renderDistance;
    }
} // Kita
