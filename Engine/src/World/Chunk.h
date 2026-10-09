#pragma once
#include "../Core/DllTemplate.h"
#include "ChunkPos.h"
#include "../Assets/AssetManager.h"

namespace Kita {
    class KITAENGINE_API Chunk {
    public:
        static constexpr int CHUNK_SIZE = 16;
        Chunk(const ChunkPos& chunkPos, AssetManager::AssetID meshAssetID);
        ChunkPos getChunkPos() const;
        AssetManager::AssetID getMeshAssetID() const;

    private:
        ChunkPos m_chunkPos;
        AssetManager::AssetID m_meshAssetID;
    };
} // Kita
