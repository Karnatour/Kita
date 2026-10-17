#include "Chunk.h"

#include "Generation/WorldGenerator.h"

namespace Kita {
    Chunk::Chunk(ChunkData chunkData) : m_chunkData(std::move(chunkData)) {
    }

    float Chunk::getWorldHeightForVertexPos(const int vertexX, const int vertexZ) const {
        return m_chunkData.getHeightmap()[(vertexZ + 1) * ChunkData::HEIGHTMAP_EDGE + (vertexX + 1)] * WorldGenerator::HEIGHT_SCALE;
    }

    float Chunk::getWorldHeightForVertexPos(const std::pair<int, int> pos) const {
        return getWorldHeightForVertexPos(pos.first, pos.second);
    }

    const ChunkData& Chunk::getChunkData() const {
        return m_chunkData;
    }

    void Chunk::setSlot(const unsigned int slot) {
        m_chunkRenderPoolSlot = slot;
    }

    bool Chunk::hasValidSlot() const {
        return m_chunkRenderPoolSlot != ChunkRenderPool::INVALID_SLOT;
    }

    unsigned int Chunk::getSlot() const {
        return m_chunkRenderPoolSlot;
    }

    void Chunk::clearSlot() {
        m_chunkRenderPoolSlot = ChunkRenderPool::INVALID_SLOT;
    }
} // Kita
