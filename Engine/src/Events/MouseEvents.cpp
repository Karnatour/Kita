#include "../kitapch.h"
#include "MouseEvents.h"

namespace Kita {
    MousePressed::MousePressed(const InputKeys::MouseButton button, const int modifiersField) : m_key(button), m_modifiersField(modifiersField) {
    }

    InputKeys::MouseButton MousePressed::getButton() const {
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

    std::pair<double, double> MouseScrolled::getOffset() const {
        return std::make_pair(m_x, m_y);
    }

    MouseMoved::MouseMoved(const double x, const double y) : m_x(x), m_y(y) {
    }

    std::pair<double, double> MouseMoved::getPosition() const {
        return std::make_pair(m_x, m_y);
    }

    MouseReleased::MouseReleased(const InputKeys::MouseButton button) : m_key(button) {
    }

    InputKeys::MouseButton MouseReleased::getButton() const {
        return m_key;
    }
} // Kita
