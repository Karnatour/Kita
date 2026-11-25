#include "QuadTree.h"

#include "../Generation/ChunkData.h"

namespace Kita {
    QuadTree::QuadTree(const float viewDistance, const LodQuality qualityLevel) : m_viewDistance(viewDistance), m_viewDistanceSq(viewDistance * viewDistance), m_qualityLevel(qualityLevel) {
        constexpr float chunkSize = static_cast<float>(ChunkData::CHUNK_SIZE);

        // Pixel perfect means everything is rendered at full detail
        if (m_qualityLevel == LodQuality::PIXEL_PERFECT) {
            m_qualityFactor = 0.0f;
            m_maxLODDetailLevel = 0;
            m_splitDistancesSquared.assign(1, 0.0f);
        } else {
            m_qualityFactor = static_cast<float>(m_qualityLevel);

            // Calculate max lod detail level
            m_maxLODDetailLevel = 1;
            while (m_qualityFactor * chunkSize * static_cast<float>(1 << m_maxLODDetailLevel) < m_viewDistance) {
                ++m_maxLODDetailLevel;
            }

            // Calculate split distances
            m_splitDistancesSquared.assign(m_maxLODDetailLevel + 1, 0.0f);
            for (unsigned int level = 1; level <= m_maxLODDetailLevel; ++level) {
                const float distance = m_qualityFactor * chunkSize * static_cast<float>(1 << level);
                m_splitDistancesSquared[level] = distance * distance;
            }
        }

        const float rootSize = chunkSize * static_cast<float>(1 << m_maxLODDetailLevel);
        m_rootRadius = static_cast<int>(std::ceil(m_viewDistance / rootSize));
    }

    void QuadTree::update(const glm::vec2 cameraPos, std::vector<QuadNode*>& out) {
        m_currentCameraPos = cameraPos;
        out.clear();
        out.reserve(getMaxNodeCount());

        const float rootSize = ChunkData::CHUNK_SIZE * static_cast<float>(1 << m_maxLODDetailLevel);
        const int camRootX = static_cast<int>(std::floor(cameraPos.x / rootSize));
        const int camRootZ = static_cast<int>(std::floor(cameraPos.y / rootSize)); // Y = Z

        for (int dz = -m_rootRadius; dz <= m_rootRadius; ++dz) {
            for (int dx = -m_rootRadius; dx <= m_rootRadius; ++dx) {
                const int rootX = camRootX + dx;
                const int rootZ = camRootZ + dz;
                auto& root = m_roots[makeKey(rootX, rootZ)];
                if (!root) {
                    root = std::make_unique<QuadNode>(m_maxLODDetailLevel, std::make_pair(rootX, rootZ));
                }
            }
        }

        for (auto it = m_roots.begin(); it != m_roots.end();) {
            const QuadNode& rootNode = *it->second;
            const auto [nodeX, nodeZ] = rootNode.getLodLevelPos();
            if (std::abs(nodeX - camRootX) > m_rootRadius || std::abs(nodeZ - camRootZ) > m_rootRadius) {
                it = m_roots.erase(it);
            } else {
                ++it;
            }
        }

        for (auto& root : m_roots | std::views::values) {
            visit(*root, out);
        }
    }

    unsigned int QuadTree::getMaxNodeCount() const {
        const unsigned int rootCount = (2 * m_rootRadius + 1) * (2 * m_rootRadius + 1);
        if (m_maxLODDetailLevel == 0) {
            return rootCount;
        }

        const unsigned int perAxis = static_cast<unsigned int>(std::floor(2.0f * m_qualityFactor)) + 2;
        const unsigned int perLevel = 4 * perAxis * perAxis;
        return rootCount + m_maxLODDetailLevel * perLevel;
    }

    void QuadTree::createChildren(QuadNode& parent) {
        if (!parent.isLeaf()) {
            return;
        }

        const auto [parentX, parentZ] = parent.getLodLevelPos();
        const int childLodLevel = parent.getLodLevel() - 1;

        auto createChild = [&](const int offsetX, const int offsetZ) {
            // TODO Node pool to avoid alloc ?
            return std::make_unique<QuadNode>(childLodLevel, std::make_pair(parentX * 2 + offsetX, parentZ * 2 + offsetZ));
        };

        parent.setChild(QuadNode::ChildIndex::TOP_LEFT, createChild(0, 0));
        parent.setChild(QuadNode::ChildIndex::TOP_RIGHT, createChild(1, 0));
        parent.setChild(QuadNode::ChildIndex::BOTTOM_LEFT, createChild(0, 1));
        parent.setChild(QuadNode::ChildIndex::BOTTOM_RIGHT, createChild(1, 1));
    }

    void QuadTree::visit(QuadNode& node, std::vector<QuadNode*>& out) {
        const float distSq = node.getAABB().getDistanceSq(m_currentCameraPos);

        if (distSq > m_viewDistanceSq) {
            node.resetChildren();
            return;
        }

        const int lodLevel = node.getLodLevel();
        const bool shouldSplit = lodLevel > 0 && distSq < m_splitDistancesSquared[lodLevel];

        if (shouldSplit) {
            createChildren(node);
            for (int i = 0; i < 4; ++i) {
                visit(*node.getChild(static_cast<QuadNode::ChildIndex>(i)), out);
            }
        } else {
            node.resetChildren();
            out.push_back(&node);
        }
    }

    // Combines X,Z into one key
    uint64_t QuadTree::makeKey(const int x, const int z) {
        return (static_cast<uint64_t>(static_cast<uint32_t>(x)) << 32) | static_cast<uint32_t>(z);
    }
} // Kita
