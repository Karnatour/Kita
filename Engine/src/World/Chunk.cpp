#include "Chunk.h"

namespace Kita {
    Chunk::Chunk(const ChunkPos& chunkPos, const AssetManager::AssetID meshAssetID) : m_chunkPos(chunkPos), m_meshAssetID(meshAssetID) {
    }

    ChunkPos Chunk::getChunkPos() const {
        return m_chunkPos;
    }

    AssetManager::AssetID Chunk::getMeshAssetID() const {
        return m_meshAssetID;
    }
} // Kita
