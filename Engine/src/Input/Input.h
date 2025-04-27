#pragma once
#include "InputKeys.h"
#include "../Events/KeyboardEvents.h"
#include "../Events/MouseEvents.h"
#include <vector>

namespace Kita {
    struct KeyWithModifier {
        KeyboardKey m_key;
        std::vector<Modifiers> m_keyboardModifiers;
    };

    struct MouseWithModifier {
        MouseButton m_button;
        std::vector<Modifiers> m_mouseModifiers;
    };

    class KITAENGINE_API Input {
    public:
        static void updateKeyPress(KeyPressed& event);
        static void updateKeyRelease(KeyReleased& event);
        static bool isKeyPressed(KeyboardKey key,std::vector<Modifiers> modifiers = {});

        static void updateButtonPress(MousePressed& event);
        static void updateButtonRelease(MouseReleased& event);
        static bool isMousePressed(MouseButton button,std::vector<Modifiers> modifiers = {});

    private:
        static std::vector<Modifiers> getSortedModifiers(int modifiersField);
        static inline std::vector<KeyWithModifier> m_keyboardPressedKeys;
        static inline std::vector<MouseWithModifier> m_mousePressedKeys;
    };
} // Kita
