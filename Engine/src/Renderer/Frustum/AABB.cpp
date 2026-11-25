#include "AABB.h"

#include <limits>

namespace Kita {
    AABB::AABB() : m_min(glm::vec3(-std::numeric_limits<float>::infinity())), m_max(std::numeric_limits<float>::infinity()) {
    }

    AABB::AABB(const glm::vec3 min, const glm::vec3 max) : m_min(min), m_max(max) {
    }

    void AABB::setPos(const glm::vec3 min, const glm::vec3 max) {
        m_min = min;
        m_max = max;
    }

    void AABB::setMin(const glm::vec3 min) {
        m_min = min;
    }

    void AABB::setMax(const glm::vec3 max) {
        m_max = max;
    }

    glm::vec3 AABB::getMin() const {
        return m_min;
    }

    glm::vec3 AABB::getMax() const {
        return m_max;
    }

    glm::vec3 AABB::getCenter() const {
        return (m_min + m_max) * 0.5f;
    }

    float AABB::getDistanceSq(const glm::vec3 point) const {
        const glm::vec3 closest = glm::clamp(point, m_min, m_max);
        const glm::vec3 delta = point - closest;

        // Dot product of same vector return distanceSq
        return glm::dot(delta, delta);
    }

    float AABB::getDistanceSqXZ(glm::vec2 point) const {
        const auto min = glm::vec2(m_min.x,m_min.z);
        const auto max = glm::vec2(m_max.x, m_min.z);

        const glm::vec2 closest = glm::clamp(point, min, max);
        const glm::vec2 delta = point - closest;

        // Dot product of same vector return distanceSq
        return glm::dot(delta, delta);
    }
} // Kita
