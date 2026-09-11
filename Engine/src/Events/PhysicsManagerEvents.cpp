#include "../kitapch.h"
#include "PhysicsManagerEvents.h"

namespace Kita {
    PreUpdateEvent::PreUpdateEvent(const double deltaTime) : m_fixedDeltaTime(deltaTime) {
    }

    double PreUpdateEvent::getFixedDeltaTime() const {
        return m_fixedDeltaTime;
    }

    PostUpdateEvent::PostUpdateEvent(const double deltaTime) : m_fixedDeltaTime(deltaTime) {
    }

    double PostUpdateEvent::getFixedDeltaTime() const {
        return m_fixedDeltaTime;
    }
} // Kita
