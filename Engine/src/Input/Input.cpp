#include "Input.h"
#include "../Events/KeyboardEvents.h"
#include "../Events/MouseEvents.h"

namespace Kita {
    void Input::updateKeyPress(KeyPressed& event) {
        m_keyboardPressedKeys.emplace_back(event.getKey(), getSortedModifiers(event.getModifiersField()));
    }

    void Input::updateKeyRelease(KeyReleased& event) {
        for (int i = 0; i < m_keyboardPressedKeys.size(); ++i) {
            if (m_keyboardPressedKeys[i].key == event.getKey()) {
                m_keyboardPressedKeys.erase(m_keyboardPressedKeys.begin() + i);
            }
        }
    }

    bool Input::isKeyPressed(const KeyboardKey key, std::vector<Modifiers> modifiers) {
        std::ranges::sort(modifiers);

        for (auto [m_key, m_keyboardModifiers] : m_keyboardPressedKeys) {
            if (m_key == key && m_keyboardModifiers == modifiers) {
                return true;
            }
        }
        return false;
    }


    void Input::updateButtonPress(MousePressed& event) {
        m_mousePressedKeys.emplace_back(event.getButton(), getSortedModifiers(event.getModifiersField()));
    }

    void Input::updateButtonRelease(MouseReleased& event) {
        for (int i = 0; i < m_mousePressedKeys.size(); ++i) {
            if (m_mousePressedKeys[i].button == event.getButton()) {
                m_mousePressedKeys.erase(m_mousePressedKeys.begin() + i);
            }
        }
    }

    bool Input::isMousePressed(const MouseButton button, std::vector<Modifiers> modifiers) {
        std::ranges::sort(modifiers);

        for (auto [m_button, m_mouseModifiers] : m_mousePressedKeys) {
            if (m_button == button && m_mouseModifiers == modifiers) {
                return true;
            }
        }
        return false;
    }

    void Input::updateMouseMovement(MouseMoved& event) {
        m_mousePos.lastMousePosition = m_mousePos.mousePosition;
        m_mousePos.mousePosition = event.getPosition();

        m_mousePos.mouseOffset.first = m_mousePos.mousePosition.first - m_mousePos.lastMousePosition.first;
        m_mousePos.mouseOffset.second = m_mousePos.mousePosition.second - m_mousePos.lastMousePosition.second;
        m_mousePos.moved = true;
    }

    bool Input::wasMouseMoved() {
        return m_mousePos.moved;
    }

    MouseMovement Input::getMousePos() {
        return m_mousePos;
    }

    void Input::updateMouseScroll(MouseScrolled& event) {
        m_mouseScroll.lastMouseScrollOffset = m_mouseScroll.mouseScrollOffset;
        m_mouseScroll.mouseScrollOffset = event.getOffset();

        m_mouseScroll.scrolled = true;
    }

    bool Input::wasMouseScrolled() {
        return m_mouseScroll.scrolled;
    }

    MouseScroll Input::getMouseScroll() {
        return m_mouseScroll;
    }

    void Input::update() {
        m_mousePos.moved = false;
        m_mouseScroll.scrolled = false;
    }

    std::vector<Modifiers> Input::getSortedModifiers(const int modifiersField) {
        std::vector<Modifiers> keyboardModifiers;
        if (modifiersField & 0x1) {
            keyboardModifiers.push_back(Modifiers::MODIFIER_SHIFT);
        }
        if (modifiersField & 0x2) {
            keyboardModifiers.push_back(Modifiers::MODIFIER_CTRL);
        }
        if (modifiersField & 0x4) {
            keyboardModifiers.push_back(Modifiers::MODIFIER_ALT);
        }
        if (modifiersField & 0x8) {
            keyboardModifiers.push_back(Modifiers::MODIFIER_SUPER);
        }
        if (modifiersField & 0x10) {
            keyboardModifiers.push_back(Modifiers::MODIFIER_CAPS_LOCK);
        }
        if (modifiersField & 0x20) {
            keyboardModifiers.push_back(Modifiers::MODIFIER_NUM_LOCK);
        }
        std::ranges::sort(keyboardModifiers);
        return keyboardModifiers;
    }
} // Kita
