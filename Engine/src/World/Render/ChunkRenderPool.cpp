#include "ChunkRenderPool.h"

#include "../../Core/Engine.h"
#include "../Generation/WorldGenerator.h"

namespace Kita {
    ChunkRenderPool::ChunkRenderPool(const unsigned int maxChunks) : m_vertexLayout(getTerrainVertexLayout()), m_maxChunks(maxChunks) {
        m_freeSlots.reserve(m_maxChunks);
        for (unsigned int i = 0; i < m_maxChunks; ++i) {
            m_freeSlots.push_back(m_maxChunks - 1 - i);
        }

        m_slotToCommand.assign(m_maxChunks, INVALID_SLOT);
        m_commands.reserve(m_maxChunks);

        constexpr int VERTICES_PER_CHUNK = WorldGenerator::VERTICES_PER_EDGE * WorldGenerator::VERTICES_PER_EDGE;
        m_meshID = Engine::getEngine()->getAssetManager().getOrCreateAsset<Mesh>(AssetKey{"Kita::ChunkRenderPool::Chunk"}, {}, VertexBufferData(std::vector<std::byte>(m_vertexLayout.getStride() * m_maxChunks * VERTICES_PER_CHUNK), m_vertexLayout),
                                                                                 createIndices()).id;

        m_drawCommandBuffer = DrawCommandBuffer::createPtr(std::vector<DrawElementsIndirectCommandProperties>(m_maxChunks));
        m_slotToAABB.assign(m_maxChunks, {});
        KITA_ENGINE_INFO("Created ChunkRenderPool with {} slots", m_maxChunks);
    }

    void ChunkRenderPool::render(const glm::vec2 camXZ, const Frustum& frustum, const float cullDistanceOffset) {
        updateCommands(camXZ);

        m_visibleCommands.clear();
        for (const auto& cmd : m_commands) {
            // ReSharper disable once CppTooWideScopeInitStatement
            const auto& AABB = m_slotToAABB[cmd.baseInstance];
            if (frustum.insideFrustum(AABB) || AABB.getDistanceSqXZ(camXZ) <= cullDistanceOffset * cullDistanceOffset) {
                m_visibleCommands.push_back(cmd);
            }
        }

        if (m_visibleCommands.empty()) {
            return;
        }

        m_drawCommandBuffer->upload(0, m_visibleCommands);

        Engine::getEngine()->getRenderer().drawTerrain(Engine::getEngine()->getAssetManager().getAsset<Mesh>(m_meshID), m_drawCommandBuffer, m_visibleCommands.size());
    }

    unsigned int ChunkRenderPool::addChunk(const NodeKey& key, const std::vector<std::byte>& vertices, std::pair<float, float> minMaxY) {
        KITA_ENGINE_PROFILE("Add chunk to render pool");
        if (m_freeSlots.empty()) {
            KITA_ENGINE_ERROR("ChunkRenderPool is full this shouldn't happen");
            return INVALID_SLOT;
        }

        const unsigned int slot = m_freeSlots.back();
        m_freeSlots.pop_back();

        auto& [minY, maxY] = minMaxY;
        const float size = static_cast<float>(ChunkData::CHUNK_SIZE * (1 << key.lodLevel));
        const glm::vec3 min(static_cast<float>(key.chunkPosX) * size, minY, static_cast<float>(key.chunkPosZ) * size);
        m_slotToAABB[slot].setPos(min, min + glm::vec3(size, maxY, size));

        const Mesh& mesh = Engine::getEngine()->getAssetManager().getAsset<Mesh>(m_meshID);

        mesh.getVertexBuffer().upload(slot * WorldGenerator::VERTICES_PER_CHUNK * m_vertexLayout.getStride(), WorldGenerator::VERTICES_PER_CHUNK * m_vertexLayout.getStride(), vertices);

        return slot;
    }

    void ChunkRenderPool::show(const unsigned int slot) {
        if (m_slotToCommand[slot] != INVALID_SLOT) {
            return; // Already rendering
        }
        const Mesh& mesh = Engine::getEngine()->getAssetManager().getAsset<Mesh>(m_meshID);
        m_slotToCommand[slot] = static_cast<uint32_t>(m_commands.size());
        m_commands.emplace_back(static_cast<unsigned int>(mesh.getIndexBuffer()->getIndices().size()), 1, 0, slot * WorldGenerator::VERTICES_PER_CHUNK, slot);
        m_sortValid = false;
    }

    void ChunkRenderPool::release(const unsigned int slot) {
        KITA_ENGINE_PROFILE("Remove chunk from render pool");
        // Move last command into remove slot
        const unsigned int commandToRemove = m_slotToCommand[slot];
        if (commandToRemove != INVALID_SLOT) {
            const unsigned int lastCommand = static_cast<unsigned int>(m_commands.size() - 1);
            if (commandToRemove != lastCommand) {
                m_commands[commandToRemove] = m_commands[lastCommand];
                m_slotToCommand[m_commands[commandToRemove].baseInstance] = commandToRemove; // Change slot
            }
            m_commands.pop_back();
            m_slotToCommand[slot] = INVALID_SLOT;
            m_sortValid = false;
        }
        m_freeSlots.push_back(slot);
    }

    unsigned int ChunkRenderPool::freeSlotCount() const {
        return static_cast<unsigned int>(m_freeSlots.size());
    }

    std::pair<size_t, size_t> ChunkRenderPool::getRenderCullingStatistic() const {
        return std::make_pair(m_visibleCommands.size(), m_commands.size());
    }

    float ChunkRenderPool::nodeDistanceSq(const unsigned int slot, const glm::vec2 camXZ) const {
        return m_slotToAABB[slot].getDistanceSqXZ(camXZ);
    }

    VertexLayout ChunkRenderPool::getTerrainVertexLayout() {
        return VertexLayout({
            VertexAttribute(VertexAttribute::Type::FLOAT, VertexAttribute::Count::THREE),
            VertexAttribute(VertexAttribute::Type::FLOAT, VertexAttribute::Count::TWO)
        });
    }

    void ChunkRenderPool::updateCommands(const glm::vec2 camXZ) {
        if (!m_sortValid || camXZ != m_sortCenter) {
            std::ranges::sort(m_commands, {}, [&](const DrawElementsIndirectCommandProperties& c) {
                return nodeDistanceSq(c.baseInstance, camXZ);
            });

            for (unsigned int i = 0; i < m_commands.size(); ++i) {
                m_slotToCommand[m_commands[i].baseInstance] = i;
            }

            m_sortCenter = camXZ;
            m_sortValid = true;
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
