#include "KeyboardEvents.h"

namespace Kita {
    KeyPressed::KeyPressed(InputKeys::KeyboardKey const key, const int modifiersField): m_key(key), m_modifiersField(modifiersField) {
    }

    InputKeys::KeyboardKey KeyPressed::getKey() const {
        return m_key;
    }

    int KeyPressed::getModifiersField() const {
        return m_modifiersField;
    }

    KeyReleased::KeyReleased(InputKeys::KeyboardKey const key) : m_key(key){
    }

    InputKeys::KeyboardKey KeyReleased::getKey() const {
        return m_key;
    }

    KeyRepeat::KeyRepeat(InputKeys::KeyboardKey const key) : m_key(key) {
    }

    InputKeys::KeyboardKey KeyRepeat::getKey() const {
        return m_key;
    }
} // Kita