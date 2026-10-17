#pragma once
#include "../../Assets/AssetManager.h"
#include "../../Core/DllTemplate.h"
#include "../../Renderer/Buffers/DrawCommandBuffer.h"
#include "../../Renderer/Properties/DrawCommandProperties.h"

namespace Kita {
    class KITAENGINE_API ChunkRenderPool {
    public:
        static constexpr unsigned int INVALID_SLOT = std::numeric_limits<unsigned int>::max();

        explicit ChunkRenderPool(unsigned int renderDistance);
        void render();
        unsigned int addChunk(const std::vector<VertexProperties>& vertices);
        void release(int slot);

    private:
        std::vector<unsigned int> createIndices();

        AssetManager::AssetID m_meshID = AssetManager::INVALID_ASSET_ID;

        std::vector<unsigned int> m_freeSlots;
        std::vector<uint32_t> m_slotToCommand; // Used for finding command based on slot

        std::vector<DrawElementsIndirectCommandProperties> m_commands;
        std::unique_ptr<DrawCommandBuffer> m_drawCommandBuffer;

        unsigned int m_maxChunks;
        bool m_dirty;
    };
} // Kita
