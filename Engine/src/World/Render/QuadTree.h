#pragma once
#include "../../Core/DllTemplate.h"
#include "QuadNode.h"

namespace Kita {
    enum class LodQuality {
        PIXEL_PERFECT = 0,
        HIGH = 8,
        NORMAL = 5,
        LOW = 3,
    };

    class KITAENGINE_API QuadTree {
    public:
        QuadTree(float viewDistance, LodQuality qualityLevel);
        void update(glm::vec2 cameraPos, std::vector<QuadNode*>& out);
        unsigned int getMaxNodeCount() const;

        QuadTree(const QuadTree&) = delete;
        QuadTree& operator=(const QuadTree&) = delete;
        QuadTree(QuadTree&&) = delete;
        QuadTree& operator=(QuadTree&&) = delete;

    private:
        void createChildren(QuadNode& parent);
        void visit(QuadNode& node, std::vector<QuadNode*>& out);
        static uint64_t makeKey(int x, int z);

        float m_viewDistance;
        float m_viewDistanceSq;
        LodQuality m_qualityLevel;
        float m_qualityFactor;
        unsigned int m_maxLODDetailLevel;
        int m_rootRadius;
        std::vector<float> m_splitDistancesSquared;

        glm::vec2 m_currentCameraPos = glm::vec2(0.0f);
        std::unordered_map<uint64_t, std::unique_ptr<QuadNode>> m_roots;
    };
} // Kita
