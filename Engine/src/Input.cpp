#include "Input.h"
#include "Events/KeyboardEvents.h"

namespace Kita {
    void Input::updateKey(const KeyPressed& event) {
        m_key = event.getKey();
        int modifiersField = event.getModifier();
        if (modifiersField & 0x1) {
            m_modifiers.emplace_back(KeyModifiers::MODIFIER_SHIFT);
        }
        if (modifiersField & 0x2) {
            m_modifiers.emplace_back(KeyModifiers::MODIFIER_CTRL);
        }
        if (modifiersField & 0x4) {
            m_modifiers.emplace_back(KeyModifiers::MODIFIER_ALT);
        }
        if (modifiersField & 0x8) {
            m_modifiers.emplace_back(KeyModifiers::MODIFIER_SUPER);
        }
        if (modifiersField & 0x10) {
            m_modifiers.emplace_back(KeyModifiers::MODIFIER_CAPS_LOCK);
        }
        if (modifiersField & 0x20) {
            m_modifiers.emplace_back(KeyModifiers::MODIFIER_NUM_LOCK);
        }
    }

    bool Input::isKeyPressed(const KeyboardKey key, const std::vector<KeyModifiers>& modifiers) {
        std::vector<KeyModifiers> sortedModifiers = modifiers;
        std::vector<KeyModifiers> sortedActive = m_modifiers;

        std::ranges::sort(sortedModifiers);
        std::ranges::sort(sortedActive);

        if (key == m_key && sortedModifiers == sortedActive) {
            m_key = KeyboardKey::KEY_NONE;
            m_modifiers.clear();
            return true;
        }

        m_key = KeyboardKey::KEY_NONE;
        m_modifiers.clear();
        return false;
    }
} // Kita
