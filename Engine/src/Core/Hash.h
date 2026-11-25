#pragma once
#include "DllTemplate.h"

namespace Kita {
    struct KITAENGINE_API Hash {
        static size_t hashCombine(size_t seed, size_t value) noexcept;
    };
} // Kita
