#pragma once
#include "InputKeys.h"
#include "../Events/KeyboardEvents.h"

namespace Kita {
    class MousePressed;

    class KITAENGINE_API Input {
    public:
        static void updateKey(const KeyPressed& event);
        static bool isKeyPressed(KeyboardKey key, const std::vector<Modifiers>& modifiers = {});

        static void updateButton(const MousePressed& event);
        static bool isMousePressed(MouseButton button, const std::vector<Modifiers>& modifiers = {});

    private:
        static inline KeyboardKey m_key = KeyboardKey::KEY_NONE;
        static inline MouseButton m_button = MouseButton::MBUTTON_NONE;
        static inline std::vector<Modifiers> m_keyboardModifiers;
        static inline std::vector<Modifiers> m_mouseModifiers;
    };
} // Kita
