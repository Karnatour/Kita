#pragma once
#include "Event.h"
#include "../Input/InputKeys.h"

namespace Kita {
    class KITAENGINE_API KeyPressed final : public Event {
    public:
        KeyPressed(KeyboardKey key, int modifiersField);
        KeyboardKey getKey() const;
        int getModifiersField() const;
    private:
        KeyboardKey m_key;
        int m_modifiersField;
    };

    class KITAENGINE_API KeyReleased final : public Event {
    public:
        explicit KeyReleased(KeyboardKey key);
        KeyboardKey getKey() const;
    private:
        KeyboardKey m_key;
    };

    class KITAENGINE_API KeyRepeat final : public Event {
    public:
        explicit KeyRepeat(KeyboardKey key);
        KeyboardKey getKey() const;
    private:
        KeyboardKey m_key;
    };

} // Kita
