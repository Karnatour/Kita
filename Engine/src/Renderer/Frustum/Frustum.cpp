#include "Frustum.h"

namespace Kita {
    // Gribb/Hartmann method
    Frustum::Frustum(const glm::mat4& viewProjection) {
        recalculate(viewProjection);
    }

    void Frustum::recalculate(const glm::mat4& viewProjection) {
        auto row = [&](const int i) {
            return glm::vec4(viewProjection[0][i], viewProjection[1][i], viewProjection[2][i], viewProjection[3][i]);
        };

        m_planes[static_cast<size_t>(PlaneIndex::LEFT)] = row(3) + row(0);
        m_planes[static_cast<size_t>(PlaneIndex::RIGHT)] = row(3) - row(0);
        m_planes[static_cast<size_t>(PlaneIndex::BOTTOM)] = row(3) + row(1);
        m_planes[static_cast<size_t>(PlaneIndex::TOP)] = row(3) - row(1);
        m_planes[static_cast<size_t>(PlaneIndex::NEAR)] = row(3) + row(2);
        m_planes[static_cast<size_t>(PlaneIndex::FAR)] = row(3) - row(2);

        // Normalize
        for (auto& plane : m_planes) {
            plane = plane / glm::length(glm::vec3(plane));
        }
    }

    glm::vec4 Frustum::getPlane(PlaneIndex index) const {
        return m_planes[static_cast<size_t>(index)];
    }

    bool Frustum::insideFrustum(const AABB& aabb) const {
        for (const auto& plane : m_planes) {
            const glm::vec3 max = aabb.getMax();
            const glm::vec3 min = aabb.getMin();
            const glm::vec3 positiveVertex(plane.x >= 0 ? max.x : min.x, plane.y >= 0 ? max.y : min.y, plane.z >= 0 ? max.z : min.z);
            if (glm::dot(glm::vec3(plane), positiveVertex) + plane.w < 0.0f) {
                return false;
            }
        }
        return true;
    }
} // Kita
