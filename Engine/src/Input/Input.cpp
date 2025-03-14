#include "../kitapch.h"
#include "Input.h"
#include "../Events/KeyboardEvents.h"
#include "../Events/MouseEvents.h"

namespace Kita {
    void Input::updateKey(const KeyPressed& event) {
        m_key = event.getKey();
        const int modifiersField = event.getModifiersField();
        if (modifiersField & 0x1) {
            m_keyboardModifiers.push_back(Modifiers::MODIFIER_SHIFT);
        }
        if (modifiersField & 0x2) {
            m_keyboardModifiers.push_back(Modifiers::MODIFIER_CTRL);
        }
        if (modifiersField & 0x4) {
            m_keyboardModifiers.push_back(Modifiers::MODIFIER_ALT);
        }
        if (modifiersField & 0x8) {
            m_keyboardModifiers.push_back(Modifiers::MODIFIER_SUPER);
        }
        if (modifiersField & 0x10) {
            m_keyboardModifiers.push_back(Modifiers::MODIFIER_CAPS_LOCK);
        }
        if (modifiersField & 0x20) {
            m_keyboardModifiers.push_back(Modifiers::MODIFIER_NUM_LOCK);
        }
    }

    bool Input::isKeyPressed(const KeyboardKey key, const std::vector<Modifiers>& modifiers) {
        std::vector<Modifiers> sortedModifiers = modifiers;
        std::vector<Modifiers> sortedActive = m_keyboardModifiers;

        std::ranges::sort(sortedModifiers);
        std::ranges::sort(sortedActive);

        if (key == m_key && sortedModifiers == sortedActive) {
            m_key = KeyboardKey::KEY_NONE;
            m_keyboardModifiers.clear();
            return true;
        }

        m_key = KeyboardKey::KEY_NONE;
        m_keyboardModifiers.clear();
        return false;
    }

    void Input::updateButton(const MousePressed& event) {
        m_button = event.getButton();
        const int modifiersField = event.getModifiersField();
        if (modifiersField & 0x1) {
            m_mouseModifiers.push_back(Modifiers::MODIFIER_SHIFT);
        }
        if (modifiersField & 0x2) {
            m_mouseModifiers.push_back(Modifiers::MODIFIER_CTRL);
        }
        if (modifiersField & 0x4) {
           m_mouseModifiers.push_back(Modifiers::MODIFIER_ALT);
        }
        if (modifiersField & 0x8) {
            m_mouseModifiers.push_back(Modifiers::MODIFIER_SUPER);
        }
        if (modifiersField & 0x10) {
            m_mouseModifiers.push_back(Modifiers::MODIFIER_CAPS_LOCK);
        }
        if (modifiersField & 0x20) {
            m_mouseModifiers.push_back(Modifiers::MODIFIER_NUM_LOCK);
        }
    }

    bool Input::isMousePressed(const MouseButton button, const std::vector<Modifiers>& modifiers) {
        std::vector<Modifiers> sortedModifiers = modifiers;
        std::vector<Modifiers> sortedActive = m_mouseModifiers;

        std::ranges::sort(sortedModifiers);
        std::ranges::sort(sortedActive);

        if (button == m_button && sortedModifiers == sortedActive) {
            m_button = MouseButton::MBUTTON_NONE;
            m_mouseModifiers.clear();
            return true;
        }

        m_button = MouseButton::MBUTTON_NONE;
        m_mouseModifiers.clear();
        return false;
    }
} // Kita
