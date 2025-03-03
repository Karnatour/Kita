#include "KeyboardEvents.h"


namespace Kita {
    KeyPressed::KeyPressed(KeyboardKey const key,int const modifier): m_key(key), m_modifiersField(modifier) {
    }

    KeyboardKey KeyPressed::getKey() const {
        return m_key;
    }

    int KeyPressed::getModifier() const {
        return m_modifiersField;
    }

    KeyReleased::KeyReleased(KeyboardKey const key) : m_key(key){
    }

    KeyboardKey KeyReleased::getKey() const {
        return m_key;
    }

    KeyRepeat::KeyRepeat(KeyboardKey const key) : m_key(key) {
    }

    KeyboardKey KeyRepeat::getKey() const {
        return m_key;
    }
} // Kita