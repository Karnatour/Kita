#pragma once
#include "../Core/DllTemplate.h"
#include <utility>

namespace Kita {
    class KITAENGINE_API ChunkPos {
    public:
        explicit ChunkPos(std::pair<int, int> chunkPos);
        ChunkPos(int x, int z);
        int getChunkX() const;
        int getChunkZ() const;
        int getVertexX(int localX) const;
        int getVertexZ(int localZ) const;
        std::pair<int, int> getChunkPos() const;

    private:
        std::pair<int, int> m_chunkPos;
    };
} // Kita
