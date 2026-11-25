#pragma once
#include "../Core/DllTemplate.h"
#include <utility>
#include <functional>

#include "../Core/Hash.h"

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
        bool isChunkInRadius(const ChunkPos& middleChunkPos, int renderDistance) const;
        static int chunkDistance(const ChunkPos& first, const ChunkPos& second);

        bool operator==(const ChunkPos& other) const = default;

    private:
        std::pair<int, int> m_chunkPos;
    };
} // Kita

template <>
struct std::hash<Kita::ChunkPos> {
    size_t operator()(const Kita::ChunkPos& chunkPos) const noexcept {
        size_t seed = 0;

        seed = Kita::Hash::hashCombine(seed, std::hash<int>{}(chunkPos.getChunkX()));
        seed = Kita::Hash::hashCombine(seed, std::hash<int>{}(chunkPos.getChunkZ()));

        return seed;
    }
};
