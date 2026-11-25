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
#include "../../../../World/Render/QuadTree.h"
#include "../../../Frustum/Frustum.h"
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

        const Frustum& getFrustum() const;

        ChunkRenderPool& getChunkRenderPool() const;

    private:
        struct PendingNode {
            NodeKey key;
            std::future<ChunkData> future;
        };

        void processGeneratedNodes();

        void updateNodes(glm::vec2 camXZ);
        void swapIfReady();
        void updateAllChunks();
        void updateChunkState(Chunk& chunk);

        int m_renderDistance = 4096;
        // Used for physics simulation currently
        int m_simulationDistance = 16;

        std::unordered_map<ChunkPos, Chunk> m_chunkMap = {};
        // Used for fast iteration
        std::vector<Chunk*> m_chunks;

        ChunkPos m_cameraChunkPos = ChunkPos(std::make_pair(0, 0));
        glm::vec3 m_cameraPos = glm::vec3(0.0f);
        std::unique_ptr<Frustum> m_frustum;

        WorldGenerator m_worldGenerator = WorldGenerator(6, 0.5f, 2.0f);
        MaterialComponent m_materialComponent;
        std::unique_ptr<ChunkRenderPool> m_chunkRenderPool;

        std::unique_ptr<QuadTree> m_quadTree;

        std::vector<QuadNode*> m_wantedNodes;
        std::vector<PendingNode> m_pendingNodeData;
        std::unordered_set<NodeKey> m_pendingNodes;
        std::unordered_set<NodeKey> m_wantedKeys;
        std::unordered_map<NodeKey, unsigned int> m_residentNodes; // Key->Slot in ChunkRenderPool

        //TODO Switch to time budget maybe instead ?
        static constexpr int ALLOWED_CHUNKS_PER_FRAME = 16;
    };
} // Kita
