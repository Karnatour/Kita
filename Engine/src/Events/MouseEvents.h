#pragma once
#include "Event.h"
#include "../Input/InputKeys.h"
#include <tuple>

namespace Kita {
    class KITAENGINE_API MousePressed final : public Event {
    public:
        MousePressed(MouseButton button, int modifiersField);
        MouseButton getButton() const;
        int getModifiersField() const;
    private:
        MouseButton m_key;
        int m_modifiersField;
    };

    class KITAENGINE_API MouseReleased final : public Event {
    public:
        explicit MouseReleased(MouseButton button);
        MouseButton getButton() const;
    private:
        MouseButton m_key;
    };

    class KITAENGINE_API MouseMoved final : public Event {
    public:
        MouseMoved(double x, double y);
        std::pair<double, double> getPosition();
    private:
        double m_x, m_y;
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
        std::pair<double, double> getOffset();
    private:
        double m_x,m_y; //Offset
    };


} // Kita
