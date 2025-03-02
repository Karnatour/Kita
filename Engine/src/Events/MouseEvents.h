#pragma once
#include "Event.h"
#include "../InputKeys.h"

namespace Kita {
    class KITAENGINE_API MousePressed final : public Event {
    public:
        explicit MousePressed(MouseKey key);
        MouseKey getKey() const;
    private:
        MouseKey m_key;
    };

    class KITAENGINE_API MouseMoved final : public Event {
    public:
        MouseMoved(int x, int y);
        std::tuple<int,int> getPosition();
    private:
        int m_x, m_y;
    };

    class KITAENGINE_API MouseChangedFocus final : public Event {
    public:
        explicit MouseChangedFocus(bool isInFocus);
        bool isInFocus() const;
    private:
        bool m_isFocused;
    };

    class KITAENGINE_API MouseScrolled final : public Event {
    public:
        MouseScrolled(double x, double y);
        std::tuple<double,double> getOffset();
    private:
        double m_x,m_y; //Offset
    };


} // Kita
