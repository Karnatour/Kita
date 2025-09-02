#pragma once
#include "Event.h"
#include "../Input/InputKeys.h"

namespace Kita {
    class KITAENGINE_API KeyPressed final : public Event {
    public:
        KeyPressed(InputKeys::KeyboardKey key, int modifiersField);
        InputKeys::KeyboardKey getKey() const;
        int getModifiersField() const;
    private:
        InputKeys::KeyboardKey m_key;
        int m_modifiersField;
    };

    class KITAENGINE_API KeyReleased final : public Event {
    public:
        explicit KeyReleased(InputKeys::KeyboardKey key);
        InputKeys::KeyboardKey getKey() const;
    private:
        InputKeys::KeyboardKey m_key;
    };

    class KITAENGINE_API KeyRepeat final : public Event {
    public:
        explicit KeyRepeat(InputKeys::KeyboardKey key);
        InputKeys::KeyboardKey getKey() const;
    private:
        InputKeys::KeyboardKey m_key;
    };

} // Kita
