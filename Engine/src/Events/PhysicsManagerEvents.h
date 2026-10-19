#pragma once
#include "Event.h"
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API PreUpdateEvent : public Event {
    public:
        explicit PreUpdateEvent(double deltaTime);
        double getFixedDeltaTime() const;
    private:
        double m_fixedDeltaTime;
    };

    class KITAENGINE_API PostUpdateEvent : public Event {
    public:
        explicit PostUpdateEvent(double deltaTime);
        double getFixedDeltaTime() const;
    private:
        double m_fixedDeltaTime;
    };
} // Kita
