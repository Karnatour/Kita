#pragma once
#include "../Core/DllTemplate.h"
#include "Generation/ChunkData.h"
#include "Render/ChunkRenderPool.h"

namespace Kita {
    class KITAENGINE_API Chunk {
    public:
        explicit Chunk(ChunkData chunkData);

        float getWorldHeightForVertexPos(int vertexX, int vertexZ) const;
        float getWorldHeightForVertexPos(std::pair<int, int> pos) const;

        const ChunkData& getChunkData() const;
        void setSlot(unsigned int slot);
        unsigned int getSlot() const;

        bool hasValidSlot() const;
        void clearSlot();

        void enableSimulation();
        void disableSimulation();

    private:
        unsigned int m_chunkRenderPoolSlot = ChunkRenderPool::INVALID_SLOT;
        ChunkData m_chunkData;
    };
} // Kita
