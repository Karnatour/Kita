#pragma once
#include <future>
#include <unordered_map>
#include <unordered_set>
#include "../../../../Core/DllTemplate.h"
#include "System.h"
#include "../../../../World/Chunk.h"
#include "../../../../World/ChunkPos.h"
#include "../../../../World/Generation/WorldGenerator.h"
#include "../Components/MaterialComponent.h"

namespace Kita {
    class KITAENGINE_API TerrainSystem : public System {
    public:
        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;

        void setRenderDistance(int renderDistance);
        int getRenderDistance();
    private:
        int m_renderDistance = 16;
        std::unordered_map<ChunkPos, Chunk> m_chunkMap = {};
        ChunkPos m_cameraChunkPos = ChunkPos(std::make_pair(0,0));
        WorldGenerator m_worldGenerator = WorldGenerator(6,0.5f,2.0f);
        MaterialComponent m_materialComponent;
        std::vector<std::future<ChunkMeshData>> m_pendingChunkMeshData;
        std::unordered_set<ChunkPos> m_pendingChunkPos;
    };
} // Kita
