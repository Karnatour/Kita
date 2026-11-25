#include "Hash.h"

namespace Kita {
    size_t Hash::hashCombine(const size_t seed, const size_t value) noexcept {
        return seed ^ value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
} // Kita
