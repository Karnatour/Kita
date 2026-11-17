#pragma once
#include "../ChunkPos.h"
#include "../../Assets/AssetManager.h"
#include "../../Core/DllTemplate.h"
#include "../../Renderer/Buffers/DrawCommandBuffer.h"
#include "../../Renderer/Properties/DrawCommandProperties.h"

namespace Kita {
    class KITAENGINE_API ChunkRenderPool {
    public:
        static constexpr unsigned int INVALID_SLOT = std::numeric_limits<unsigned int>::max();

        explicit ChunkRenderPool(const ChunkPos& centerPos, unsigned int renderDistance);
        void render(const ChunkPos& center);
        void renderInDistance(const ChunkPos& center, unsigned int renderDistance);
        unsigned int addChunk(const ChunkPos& pos, const std::vector<std::byte>& vertices);
        void release(int slot);

    private:
        void updateCommands(const ChunkPos& center);
        std::vector<unsigned int> createIndices();

        AssetManager::AssetID m_meshID = AssetManager::INVALID_ASSET_ID;

        std::vector<unsigned int> m_freeSlots;
        std::vector<uint32_t> m_slotToCommand; // Used for finding command based on slot
        std::vector<ChunkPos> m_slotToPos; // Used for finding chunk pos based on slot

        std::vector<DrawElementsIndirectCommandProperties> m_commands;
        std::unique_ptr<DrawCommandBuffer> m_drawCommandBuffer;

        ChunkPos m_sortCenter;

        VertexLayout m_vertexLayout;
        unsigned int m_maxChunks = 0;
        bool m_dirty = false;
        bool m_sortValid = false;
    };
} // Kita
