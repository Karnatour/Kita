#include "Input.h"
#include "Events/KeyboardEvents.h"

namespace Kita {
    void Input::updateKey(const KeyPressed& event) {
        m_key = event.getKey();
    }

    bool Input::isKeyPressed(KeyboardKey const key) {
        if (key == m_key) {
            m_key = KeyboardKey::KEY_NONE;
            return true;
        }
        return false;
    }
} // Kita
