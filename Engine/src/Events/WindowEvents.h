#pragma once
#include "Event.h"
#include <tuple>

namespace Kita {
    class KITAENGINE_API WindowMoved final : public Event {
    public:
        WindowMoved(int x, int y);
        std::tuple<int,int> getPosition();

        int m_x, m_y;
    };

    class KITAENGINE_API WindowResized final : public Event {
    public:
        WindowResized(int width, int height);
        std::tuple <int,int> getPosition();
    private:
        int m_width,m_height;
    };

    class KITAENGINE_API WindowClosed final : public Event {
    };

    class KITAENGINE_API WindowChangedFocus final : public Event {
    public:
        explicit WindowChangedFocus(bool isFocused);
        bool isFocused() const;
    private:
        bool m_isFocused;
    };
} // Kita
