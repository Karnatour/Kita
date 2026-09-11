#pragma once
#include "Event.h"

namespace Kita {
    class PreUpdateEvent : public Event {
    public:
        explicit PreUpdateEvent(double deltaTime);
        double getFixedDeltaTime() const;
    private:
        double m_fixedDeltaTime;
    };

    class PostUpdateEvent : public Event {
    public:
        explicit PostUpdateEvent(double deltaTime);
        double getFixedDeltaTime() const;
    private:
        double m_fixedDeltaTime;
    };
} // Kita
