#pragma once
#include "InputKeys.h"
#include "Events/KeyboardEvents.h"

namespace Kita {

class KITAENGINE_API Input {
public:
    static void updateKey(const KeyPressed& event);
    static bool isKeyPressed(KeyboardKey key);
private:
    static inline KeyboardKey m_key = KeyboardKey::KEY_NONE;
};

} // Kita
