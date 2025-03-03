#pragma once
#include "InputKeys.h"
#include "Events/KeyboardEvents.h"

namespace Kita {
    class KITAENGINE_API Input {
    public:
        static void updateKey(const KeyPressed& event);

        static bool isKeyPressed(KeyboardKey key,const std::vector<KeyModifiers>& modifiers = {});

    private:
        static inline KeyboardKey m_key = KeyboardKey::KEY_NONE;
        static inline std::vector<KeyModifiers> m_modifiers;
    };
} // Kita
