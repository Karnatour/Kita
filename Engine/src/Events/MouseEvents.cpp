#include "MouseEvents.h"

namespace Kita {
    MousePressed::MousePressed(const MouseKey key) : m_key(key) {
    }

    MouseKey MousePressed::getKey() const {
        return m_key;
    }

    MouseChangedFocus::MouseChangedFocus(const bool isInFocus): m_isFocused(isInFocus) {
    }

    bool MouseChangedFocus::isInFocus() const {
        return m_isFocused;
    }

    MouseScrolled::MouseScrolled(const double x, const double y): m_x(x), m_y(y) {
    }

    std::tuple<double, double> MouseScrolled::getOffset() {
        return std::make_tuple(m_x,m_y);
    }

    MouseMoved::MouseMoved(const int x, const int y) : m_x(x), m_y(y) {
    }

    std::tuple<int, int> MouseMoved::getPosition() {
        return std::make_tuple(m_x, m_y);
    }
} // Kita
