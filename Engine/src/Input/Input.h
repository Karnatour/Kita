#pragma once
#include "InputKeys.h"
#include "../Events/KeyboardEvents.h"
#include "../Events/MouseEvents.h"
#include <vector>

namespace Kita {
    struct KeyWithModifier {
        InputKeys::KeyboardKey key;
        std::vector<InputKeys::Modifiers> keyboardModifiers;
    };

    struct MouseWithModifier {
        InputKeys::MouseButton button;
        std::vector<InputKeys::Modifiers> mouseModifiers;
    };

    struct MouseMovement {
        std::pair<double, double> lastMousePosition;
        std::pair<double, double> mousePosition;
        std::pair<double, double> mouseOffset;
        bool moved = false;
    };

    struct MouseScroll {
        std::pair<double, double> lastMouseScrollOffset;
        std::pair<double, double> mouseScrollOffset;
        bool scrolled = false;
    };

    class KITAENGINE_API Input {
    public:
        static bool isKeyPressed(InputKeys::KeyboardKey key, std::vector<InputKeys::Modifiers> modifiers = {});
        static bool isMousePressed(InputKeys::MouseButton button, std::vector<InputKeys::Modifiers> modifiers = {});

        static bool wasMouseMoved();
        static MouseMovement getMousePos();

        static bool wasMouseScrolled();
        static MouseScroll getMouseScroll();
    private:
        friend class Engine;

        static void init();
        static void update();
        static void updateKeyPress(const KeyPressed& event);
        static void updateKeyRelease(const KeyReleased& event);
        static void updateButtonPress(const MousePressed& event);
        static void updateButtonRelease(const MouseReleased& event);
        static void updateMouseMovement(const MouseMoved& event);
        static void updateMouseScroll(const MouseScrolled& event);

        static std::vector<InputKeys::Modifiers> getSortedModifiers(int modifiersField);

        static inline std::vector<KeyWithModifier> m_keyboardPressedKeys;
        static inline std::vector<MouseWithModifier> m_mousePressedKeys;
        static inline MouseMovement m_mousePos;
        static inline MouseScroll m_mouseScroll;

    };
} // Kita
