#pragma once
#include <vector>

#include "../ChunkPos.h"

namespace Kita {
    struct VertexProperties;

    class ChunkMeshData {
    public:
        ChunkMeshData(const ChunkPos& chunkPos, std::vector<VertexProperties> vertices, std::vector<unsigned int> indices);
        ChunkPos getChunkPos();
        std::vector<VertexProperties>& getVertices();
        std::vector<unsigned int>& getIndices();
    private:
        ChunkPos m_chunkPos;
        std::vector<VertexProperties> m_vertices;
        std::vector<unsigned int> m_indices;
    };
} // Kita
