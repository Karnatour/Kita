#pragma once
#include "NodeKey.h"
#include "QuadNodeAABB.h"
#include "../../Renderer/Frustum/Frustum.h"
#include "../../Assets/AssetManager.h"
#include "../../Core/DllTemplate.h"
#include "../../Renderer/Buffers/DrawCommandBuffer.h"
#include "../../Renderer/Properties/DrawCommandProperties.h"

namespace Kita {
    class KITAENGINE_API ChunkRenderPool {
    public:
        static constexpr unsigned int INVALID_SLOT = std::numeric_limits<unsigned int>::max();

        explicit ChunkRenderPool(unsigned int maxChunks);
        void render(glm::vec2 camXZ, const Frustum& frustum);
        //void renderInDistance(glm::vec2 camXZ, unsigned int renderDistance);
        unsigned int addChunk(const NodeKey& key, const std::vector<std::byte>& vertices);
        void show(unsigned int slot);
        void release(unsigned int slot);
        unsigned int freeSlotCount() const;

    private:
        float nodeDistanceSq(unsigned int slot, glm::vec2 camXZ) const;
        VertexLayout getTerrainVertexLayout();
        void updateCommands(glm::vec2 camXZ);
        std::vector<unsigned int> createIndices();

        AssetManager::AssetID m_meshID = AssetManager::INVALID_ASSET_ID;

        std::vector<unsigned int> m_freeSlots;
        std::vector<uint32_t> m_slotToCommand; // Used for finding command based on slot
        std::vector<AABB> m_slotToAABB; // Used for sorting

        std::vector<DrawElementsIndirectCommandProperties> m_commands;
        std::vector<DrawElementsIndirectCommandProperties> m_visibleCommands;
        std::unique_ptr<DrawCommandBuffer> m_drawCommandBuffer;

        glm::vec2 m_sortCenter = glm::vec2(0.0f);

        VertexLayout m_vertexLayout;
        unsigned int m_maxChunks = 0;
        bool m_sortValid = false;
    };
} // Kita
