#pragma once
#include "../Core/DllTemplate.h"
#include "ChunkPos.h"

namespace Kita {
    class KITAENGINE_API Chunk {
    public:
        static constexpr int CHUNK_SIZE = 16;
        explicit Chunk(const ChunkPos& chunkPos);
        ChunkPos getChunkPos();
    private:
        ChunkPos m_chunkPos;
    };
} // Kita
