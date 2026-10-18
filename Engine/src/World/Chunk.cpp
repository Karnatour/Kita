#include "Chunk.h"

#include "../Core/Engine.h"
#include "Generation/WorldGenerator.h"

namespace Kita {
    Chunk::Chunk(ChunkData chunkData) : m_chunkData(std::move(chunkData)) {
    }

    float Chunk::getWorldHeightForVertexPos(const int vertexX, const int vertexZ) const {
        return m_chunkData.getHeightmap()[(vertexZ + 1) * ChunkData::HEIGHTMAP_EDGE + (vertexX + 1)] * WorldGenerator::HEIGHT_SCALE;
    }

    float Chunk::getWorldHeightForVertexPos(const std::pair<int, int> pos) const {
        return getWorldHeightForVertexPos(pos.first, pos.second);
    }

    const ChunkData& Chunk::getChunkData() const {
        return m_chunkData;
    }

    void Chunk::setSlot(const unsigned int slot) {
        m_chunkRenderPoolSlot = slot;
    }

    bool Chunk::hasValidSlot() const {
        return m_chunkRenderPoolSlot != ChunkRenderPool::INVALID_SLOT;
    }

    unsigned int Chunk::getSlot() const {
        return m_chunkRenderPoolSlot;
    }

    void Chunk::clearSlot() {
        m_chunkRenderPoolSlot = ChunkRenderPool::INVALID_SLOT;
    }

    void Chunk::enableSimulation() {
        if (m_chunkData.getBodyID() != JPH::BodyID(JPH::BodyID::cInvalidBodyID)) {
            return;
        }

        auto& physicsManager = Engine::getEngine()->getPhysicsManager();
        m_chunkData.setBodyID(physicsManager.createBody(m_chunkData.getBodyCreationSettings(), JPH::EActivation::DontActivate));
    }

    void Chunk::disableSimulation() {
        const auto bodyID = m_chunkData.getBodyID();
        if (bodyID == JPH::BodyID(JPH::BodyID::cInvalidBodyID)) {
            return;
        }

        auto& physicsManager = Engine::getEngine()->getPhysicsManager();
        physicsManager.removeBody(bodyID);
        physicsManager.destroyBody(bodyID);
        m_chunkData.setBodyID(JPH::BodyID(JPH::BodyID::cInvalidBodyID));
    }
} // Kita
