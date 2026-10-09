#pragma once
#include "../Core/DllTemplate.h"
#include <utility>
#include <functional>

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

        bool operator==(const ChunkPos& other) const = default;

    private:
        std::pair<int, int> m_chunkPos;
    };
} // Kita

template <>
struct std::hash<Kita::ChunkPos> {
    std::size_t operator()(const Kita::ChunkPos& chunkPos) const noexcept {
        const std::size_t h1 = std::hash<int>{}(chunkPos.getChunkX());
        const std::size_t h2 = std::hash<int>{}(chunkPos.getChunkZ());

        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};
