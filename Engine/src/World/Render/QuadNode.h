#pragma once
#include "NodeKey.h"
#include "QuadNodeAABB.h"
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API QuadNode {
    public:
        enum class ChildIndex {
            TOP_LEFT = 0,
            TOP_RIGHT = 1,
            BOTTOM_LEFT = 2,
            BOTTOM_RIGHT = 3
        };

        QuadNode() = default;
        QuadNode(int lodLevel, std::pair<int, int> lodLevelPos);
        int getLodLevel() const;
        bool isLeaf() const;
        int getSize() const;
        float getWorldX() const;
        float getWorldZ() const;
        std::pair<int, int> getLodLevelPos() const;
        glm::vec2 getWorldPos() const;
        const QuadNodeAABB& getAABB() const;
        NodeKey getKey() const;

        QuadNode* getChild(ChildIndex index);
        void setChild(ChildIndex index, std::unique_ptr<QuadNode> child);

        void resetChildren();

    private:
        int m_lodLevel = 0;
        std::array<std::unique_ptr<QuadNode>, 4> m_children; //TopLeft,TopRight,BottomLeft,BottomRight
        std::pair<int, int> m_lodLevelPos = {0, 0};
        QuadNodeAABB m_aabb;
    };
} // Kita
