#include "ChunkMeshData.h"
#include "../../Renderer/Properties/VertexProperties.h"

namespace Kita {
    ChunkMeshData::ChunkMeshData(const ChunkPos& chunkPos, std::vector<VertexProperties> vertices, std::vector<unsigned int> indices) : m_chunkPos(chunkPos), m_vertices(std::move(vertices)), m_indices(std::move(indices)) {
    }

    ChunkPos ChunkMeshData::getChunkPos() {
        return m_chunkPos;
    }

    std::vector<VertexProperties>& ChunkMeshData::getVertices() {
        return m_vertices;
    }

    std::vector<unsigned int>& ChunkMeshData::getIndices() {
        return m_indices;
    }
} // Kita
