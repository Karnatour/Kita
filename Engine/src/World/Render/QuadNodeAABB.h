#pragma once
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API QuadNodeAABB {
    public:
        QuadNodeAABB();
        QuadNodeAABB(glm::vec2 min, glm::vec2 max);

        void setPos(glm::vec2 min, glm::vec2 max);

        void setMin(glm::vec2 min);
        void setMax(glm::vec2 max);

        glm::vec2 getMin() const;
        glm::vec2 getMax() const;

        glm::vec2 getCenter() const;
        float getDistanceSq(glm::vec2 point) const;

    private:
        glm::vec2 m_min;
        glm::vec2 m_max;
    };
} // Kita
