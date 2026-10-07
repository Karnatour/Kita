#include "Chunk.h"

namespace Kita {
    Chunk::Chunk(const ChunkPos& chunkPos) : m_chunkPos(chunkPos) {
    }

    ChunkPos Chunk::getChunkPos() {
        return m_chunkPos;
    }
} // Kita
