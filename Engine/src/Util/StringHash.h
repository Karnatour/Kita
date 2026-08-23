#pragma once
#include "../Core/DllTemplate.h"
#include <string>

namespace Kita {
    struct KITAENGINE_API StringHash {
        using is_transparent = void;
        size_t operator()(const std::string_view sv) const {
            return std::hash<std::string_view>{}(sv);
        }
    };
} // Kita