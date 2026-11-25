#pragma once
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API AABB {
    public:
        AABB();
        AABB(glm::vec3 min, glm::vec3 max);

        void setPos(glm::vec3 min, glm::vec3 max);

        void setMin(glm::vec3 min);
        void setMax(glm::vec3 max);

        glm::vec3 getMin() const;
        glm::vec3 getMax() const;

        glm::vec3 getCenter() const;
        float getDistanceSq(glm::vec3 point) const;
        float getDistanceSqXZ(glm::vec2 point) const;

    private:
        glm::vec3 m_min;
        glm::vec3 m_max;
    };
} // Kita
