#include "QuadNode.h"

#include "../Generation/ChunkData.h"

namespace Kita {
    QuadNode::QuadNode(const int lodLevel, const std::pair<int, int> lodLevelPos) : m_lodLevel(lodLevel), m_lodLevelPos(lodLevelPos) {
        const glm::vec2 min = getWorldPos();
        const float size = static_cast<float>(getSize());
        m_aabb.setPos(min, min + glm::vec2(size));
    }

    int QuadNode::getLodLevel() const {
        return m_lodLevel;
    }

    bool QuadNode::isLeaf() const {
        return !m_children[static_cast<size_t>(ChildIndex::TOP_LEFT)];
    }

    int QuadNode::getSize() const {
        return ChunkData::CHUNK_SIZE * (1 << m_lodLevel);
    }

    float QuadNode::getWorldX() const {
        return static_cast<float>(m_lodLevelPos.first) * getSize();
    }

    float QuadNode::getWorldZ() const {
        return static_cast<float>(m_lodLevelPos.second) * getSize();
    }

    std::pair<int, int> QuadNode::getLodLevelPos() const {
        return m_lodLevelPos;
    }

    glm::vec2 QuadNode::getWorldPos() const {
        return glm::vec2(getWorldX(), getWorldZ());
    }

    const QuadNodeAABB& QuadNode::getAABB() const {
        return m_aabb;
    }

    NodeKey QuadNode::getKey() const {
        return NodeKey{.lodLevel = m_lodLevel, .chunkPosX = m_lodLevelPos.first, .chunkPosZ = m_lodLevelPos.second};
    }

    QuadNode* QuadNode::getChild(ChildIndex index) {
        return m_children[static_cast<size_t>(index)].get();
    }

    void QuadNode::setChild(const ChildIndex index, std::unique_ptr<QuadNode> child) {
        m_children[static_cast<size_t>(index)] = std::move(child);
    }

    void QuadNode::resetChildren() {
        for (auto& child : m_children) {
            child.reset();
        }
    }
} // Kita
