#include "../kitapch.h"
#include "WindowEvents.h"

namespace Kita {
    WindowMoved::WindowMoved(const int x, const int y): m_x(x), m_y(y) {
    }

    std::tuple<int, int> WindowMoved::getPosition() {
        return std::make_tuple(m_x,m_y);
    }

    WindowResized::WindowResized(const int width, const int height): m_width(width), m_height(height) {
    }

    std::tuple<int, int> WindowResized::getPosition() {
        return std::make_tuple(m_width,m_height);
    }

    WindowChangedFocus::WindowChangedFocus(const bool isFocused): m_isFocused(isFocused) {
    }

    bool WindowChangedFocus::isFocused() const {
        return m_isFocused;
    }
} // Kita