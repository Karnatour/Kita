#pragma once
#include <string_view>

#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API DebugScope {
    public:
        DebugScope() = default;
        virtual ~DebugScope() = default;

        DebugScope(const DebugScope&) = delete;
        DebugScope& operator=(const DebugScope&) = delete;
        DebugScope(DebugScope&&) = delete;
        DebugScope& operator=(DebugScope&&) = delete;

        [[nodiscard]] static std::unique_ptr<DebugScope> createPtr();
        virtual void scope(std::string_view name) = 0;
    };
} // Kita
