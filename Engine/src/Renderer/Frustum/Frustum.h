#pragma once
#include "AABB.h"

namespace Kita {
    class Frustum {
    public:
        enum class PlaneIndex {
            LEFT = 0,
            RIGHT = 1,
            BOTTOM = 2,
            TOP = 3,
            NEAR = 4,
            FAR = 5
        };

        explicit Frustum(const glm::mat4& viewProjection);

        void recalculate(const glm::mat4& viewProjection);
        glm::vec4 getPlane(PlaneIndex index) const;
        bool insideFrustum(const AABB& aabb) const;

    private:
        std::array<glm::vec4, 6> m_planes;
    };
} // Kita
