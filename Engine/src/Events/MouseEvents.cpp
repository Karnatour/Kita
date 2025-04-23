#include "MouseEvents.h"

namespace Kita {
    MousePressed::MousePressed(const MouseButton button, const int modifiersField) : m_key(button), m_modifiersField(modifiersField) {
    }

    MouseButton MousePressed::getButton() const {
        return m_key;
    }

    int MousePressed::getModifiersField() const {
        return m_modifiersField;
    }

    MouseChangedFocus::MouseChangedFocus(const bool isInFocus): m_isFocused(isInFocus) {
    }

    bool MouseChangedFocus::isInFocus() const {
        return m_isFocused;
    }

    MouseScrolled::MouseScrolled(const double x, const double y): m_x(x), m_y(y) {
    }

    std::pair<double, double> MouseScrolled::getOffset() {
        return std::make_pair(m_x, m_y);
    }

    MouseMoved::MouseMoved(const double x, const double y) : m_x(x), m_y(y) {
    }

    std::pair<double, double> MouseMoved::getPosition() {
        return std::make_pair(m_x, m_y);
    }

    MouseReleased::MouseReleased(const MouseButton button) : m_key(button) {
    }

    MouseButton MouseReleased::getButton() const {
        return m_key;
    }
} // Kita
