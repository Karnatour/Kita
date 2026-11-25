#include "QuadNodeAABB.h"

namespace Kita {
    QuadNodeAABB::QuadNodeAABB() : m_min(glm::vec2(-std::numeric_limits<float>::infinity())), m_max(std::numeric_limits<float>::infinity()){
    }

    QuadNodeAABB::QuadNodeAABB(const glm::vec2 min, const glm::vec2 max) : m_min(min), m_max(max) {
    }

    void QuadNodeAABB::setPos(const glm::vec2 min,const glm::vec2 max) {
        m_min = min;
        m_max = max;
    }

    void QuadNodeAABB::setMin(const glm::vec2 min) {
        m_min = min;
    }

    void QuadNodeAABB::setMax(const glm::vec2 max) {
        m_max = max;
    }

    glm::vec2 QuadNodeAABB::getMin() const {
        return m_min;
    }

    glm::vec2 QuadNodeAABB::getMax() const {
        return m_max;
    }

    glm::vec2 QuadNodeAABB::getCenter() const {
        return (m_min + m_max) * 0.5f;
    }

    float QuadNodeAABB::getDistanceSq(const glm::vec2 point) const {
        const glm::vec2 closest = glm::clamp(point, m_min, m_max);
        const glm::vec2 delta = point - closest;

        // Dot product of same vector return distanceSq
        return glm::dot(delta, delta);
    }
} // Kita
