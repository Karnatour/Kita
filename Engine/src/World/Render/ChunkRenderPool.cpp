#include "ChunkRenderPool.h"

#include "../Chunk.h"
#include "../../Core/Engine.h"
#include "../Generation/WorldGenerator.h"

namespace Kita {
    std::vector<unsigned int> ChunkRenderPool::createIndices() {
        std::vector<unsigned int> indices;
        indices.reserve(ChunkData::CHUNK_QUAD_PER_EDGE * ChunkData::CHUNK_QUAD_PER_EDGE * 6);

        for (int z = 0; z < ChunkData::CHUNK_QUAD_PER_EDGE; ++z) {
            for (int x = 0; x < ChunkData::CHUNK_QUAD_PER_EDGE; ++x) {
                const unsigned int topLeft = z * WorldGenerator::VERTICES_PER_EDGE + x;
                const unsigned int topRight = topLeft + 1;
                const unsigned int bottomLeft = topLeft + WorldGenerator::VERTICES_PER_EDGE;
                const unsigned int bottomRight = bottomLeft + 1;

                // Triangle left
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                // Triangle right
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
        return indices;
    }

    ChunkRenderPool::ChunkRenderPool(const unsigned int renderDistance) {
        m_maxChunks = static_cast<unsigned int>(std::pow(2 * renderDistance + 1, 2)) + renderDistance * renderDistance;

        m_freeSlots.reserve(m_maxChunks);
        for (unsigned int i = 0; i < m_maxChunks; ++i) {
            m_freeSlots.push_back(m_maxChunks - 1 - i);
        }

        m_slotToCommand.assign(m_maxChunks, INVALID_SLOT);
        m_commands.reserve(m_maxChunks);

        m_meshID = Engine::getEngine()->getAssetManager().createAsset<Mesh>(std::vector<VertexProperties>(), createIndices());
        Mesh& mesh = Engine::getEngine()->getAssetManager().getAsset<Mesh>(m_meshID);
        mesh.getVertexBuffer().createEmptyBuffer(m_maxChunks * (WorldGenerator::VERTICES_PER_EDGE * WorldGenerator::VERTICES_PER_EDGE) * sizeof(VertexProperties));
        mesh.getVertexArray().attachVBO(mesh.getVertexBuffer());

        m_drawCommandBuffer = DrawCommandBuffer::createPtr();
        m_drawCommandBuffer->createEmptyBuffer(DrawCommandBuffer::Type::ELEMENTS, m_maxChunks * sizeof(DrawElementsIndirectCommandProperties));
    }

    void ChunkRenderPool::render() {
        if (m_dirty) {
            if (!m_commands.empty()) {
                m_drawCommandBuffer->upload(0, m_commands);
            }
            m_dirty = false;
        }

        if (m_commands.empty()) {
            return;
        }

        Engine::getEngine()->getRenderer().drawTerrain(Engine::getEngine()->getAssetManager().getAsset<Mesh>(m_meshID), m_drawCommandBuffer, m_commands.size());
    }

    unsigned int ChunkRenderPool::addChunk(const std::vector<VertexProperties>& vertices) {
        const unsigned int slot = m_freeSlots.back();
        m_freeSlots.pop_back();

        const Mesh& mesh = Engine::getEngine()->getAssetManager().getAsset<Mesh>(m_meshID);

        constexpr int VERTICES_PER_CHUNK = WorldGenerator::VERTICES_PER_EDGE * WorldGenerator::VERTICES_PER_EDGE;
        mesh.getVertexBuffer().upload(slot * VERTICES_PER_CHUNK * sizeof(VertexProperties), VERTICES_PER_CHUNK * sizeof(VertexProperties), vertices);

        m_slotToCommand[slot] = static_cast<uint32_t>(m_commands.size());
        m_commands.emplace_back(static_cast<unsigned int>(mesh.getIndexBuffer()->getIndices().size()), 1, 0, (slot * VERTICES_PER_CHUNK), slot); // Since we are not using baseInstance we can store slot into it

        m_dirty = true;
        return slot;
    }

    void ChunkRenderPool::release(const int slot) {
        // Move last command into remove slot
        const unsigned int commandToRemove = m_slotToCommand[slot];
        const unsigned int lastCommand = static_cast<unsigned int>(m_commands.size() - 1);

        if (commandToRemove != lastCommand) {
            m_commands[commandToRemove] = m_commands[lastCommand];
            m_slotToCommand[m_commands[commandToRemove].baseInstance] = commandToRemove; // Change slot
        }

        m_commands.pop_back();
        m_slotToCommand[slot] = INVALID_SLOT;

        m_freeSlots.push_back(slot);
        m_dirty = true;
    }
} // Kita
