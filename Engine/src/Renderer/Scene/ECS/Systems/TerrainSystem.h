#pragma once
#include <future>
#include <unordered_map>
#include <unordered_set>
#include "../../../../Core/DllTemplate.h"
#include "System.h"
#include "../../../../World/Chunk.h"
#include "../../../../World/ChunkPos.h"
#include "../../../../World/Generation/WorldGenerator.h"
#include "../../../../World/Render/ChunkRenderPool.h"
#include "../Components/MaterialComponent.h"

namespace Kita {
    class KITAENGINE_API TerrainSystem : public System {
    public:
        explicit TerrainSystem(Scene& scene)
            : System(scene) {
        }

        TerrainSystem(const TerrainSystem&) = delete;
        TerrainSystem& operator=(const TerrainSystem&) = delete;

        TerrainSystem(TerrainSystem&&) noexcept = delete;
        TerrainSystem& operator=(TerrainSystem&&) noexcept = delete;

        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;

        void setRenderDistance(int renderDistance);
        int getRenderDistance() const;

        const Chunk* getChunkForPos(const ChunkPos& chunkPos) const;
        const Chunk* getCameraChunk() const;
        ChunkPos getCameraChunkPos() const;

    private:
        void generateChunkMeshData();
        void processGeneratedChunkMeshData();
        void removeOutOfRangeChunks();
        void addInRangeChunks();

        int m_renderDistance = 2;
        std::unordered_map<ChunkPos, Chunk> m_chunkMap = {};

        ChunkPos m_cameraChunkPos = ChunkPos(std::make_pair(0, 0));
        WorldGenerator m_worldGenerator = WorldGenerator(6, 0.5f, 2.0f);
        MaterialComponent m_materialComponent;
        std::unique_ptr<ChunkRenderPool> m_chunkRenderPool;

        std::vector<std::future<ChunkData>> m_pendingChunkMeshData;
        std::unordered_set<ChunkPos> m_pendingChunkPos;

        //TODO Switch to time budget maybe instead ?
        static constexpr int ALLOWED_CHUNKS_PER_FRAME = 16;
    };
} // Kita
