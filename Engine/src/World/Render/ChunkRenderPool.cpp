#include "ChunkRenderPool.h"

#include "../../Core/Engine.h"
#include "../Generation/WorldGenerator.h"

namespace Kita {
    ChunkRenderPool::ChunkRenderPool(const ChunkPos& centerPos, const unsigned int renderDistance) :
        m_sortCenter(centerPos), m_vertexLayout(VertexLayout({
            VertexAttribute(VertexAttribute::Type::FLOAT, VertexAttribute::Count::THREE),
            VertexAttribute(VertexAttribute::Type::FLOAT, VertexAttribute::Count::TWO)
        })) {
        m_maxChunks = static_cast<unsigned int>(std::pow(2 * renderDistance + 1, 2));
        m_freeSlots.reserve(m_maxChunks);
        for (unsigned int i = 0; i < m_maxChunks; ++i) {
            m_freeSlots.push_back(m_maxChunks - 1 - i);
        }

        m_slotToCommand.assign(m_maxChunks, INVALID_SLOT);
        m_commands.reserve(m_maxChunks);

        constexpr int VERTICES_PER_CHUNK = WorldGenerator::VERTICES_PER_EDGE * WorldGenerator::VERTICES_PER_EDGE;
        m_meshID = Engine::getEngine()->getAssetManager().getOrCreateAsset<Mesh>(AssetKey{"Kita::ChunkRenderPool::Chunk"},{},VertexBufferData(std::vector<std::byte>(m_vertexLayout.getStride() * m_maxChunks * VERTICES_PER_CHUNK), m_vertexLayout), createIndices()).id;

        m_drawCommandBuffer = DrawCommandBuffer::createPtr(std::vector<DrawElementsIndirectCommandProperties>(m_maxChunks));
        m_slotToPos.assign(m_maxChunks, ChunkPos(0, 0));
    }

    void ChunkRenderPool::render(const ChunkPos& center) {
        updateCommands(center);
        if (m_commands.empty()) {
            return;
        }

        Engine::getEngine()->getRenderer().drawTerrain(Engine::getEngine()->getAssetManager().getAsset<Mesh>(m_meshID), m_drawCommandBuffer, m_commands.size());
    }

    void ChunkRenderPool::renderInDistance(const ChunkPos& center, const unsigned int renderDistance) {
        updateCommands(center);

        const auto end = std::ranges::partition_point(m_commands, [&](const auto& c) {
            return ChunkPos::chunkDistance(m_slotToPos[c.baseInstance], center) <= static_cast<int>(renderDistance);
        });

        const size_t count = std::ranges::distance(m_commands.begin(), end);
        if (count == 0) {
            return;
        };

        Engine::getEngine()->getRenderer().drawTerrain(Engine::getEngine()->getAssetManager().getAsset<Mesh>(m_meshID), m_drawCommandBuffer, count);
    }

    unsigned int ChunkRenderPool::addChunk(const ChunkPos& pos, const std::vector<std::byte>& vertices) {
        if (m_freeSlots.empty()) {
            KITA_ENGINE_ERROR("ChunkRenderPool is full this shouldn't happen");
            return INVALID_SLOT;
        }

        const unsigned int slot = m_freeSlots.back();
        m_freeSlots.pop_back();
        m_slotToPos[slot] = pos;

        const Mesh& mesh = Engine::getEngine()->getAssetManager().getAsset<Mesh>(m_meshID);

        mesh.getVertexBuffer().upload(slot * WorldGenerator::VERTICES_PER_CHUNK * m_vertexLayout.getStride(), WorldGenerator::VERTICES_PER_CHUNK * m_vertexLayout.getStride(), vertices);

        m_slotToCommand[slot] = static_cast<uint32_t>(m_commands.size());
        m_commands.emplace_back(static_cast<unsigned int>(mesh.getIndexBuffer()->getIndices().size()), 1, 0, (slot * WorldGenerator::VERTICES_PER_CHUNK), slot); // Since we are not using baseInstance we can store slot into it

        m_dirty = true;
        m_sortValid = false;
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
        m_sortValid = false;
    }

    void ChunkRenderPool::updateCommands(const ChunkPos& center) {
        if (!m_sortValid || center != m_sortCenter) {
            auto key = [&](const DrawElementsIndirectCommandProperties& c) {
                const ChunkPos& p = m_slotToPos[c.baseInstance];
                const int dx = p.getChunkX() - center.getChunkX();
                const int dz = p.getChunkZ() - center.getChunkZ();
                return std::make_pair(ChunkPos::chunkDistance(p, center), dx * dx + dz * dz);
            };
            std::ranges::sort(m_commands, {}, key);

            for (unsigned int i = 0; i < m_commands.size(); ++i) {
                m_slotToCommand[m_commands[i].baseInstance] = i;
            }

            m_sortCenter = center;
            m_sortValid = true;
            m_dirty = true;
        }

        if (m_dirty) {
            if (!m_commands.empty()) {
                m_drawCommandBuffer->upload(0, m_commands);
            }
            m_dirty = false;
        }
    }

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
} // Kita
