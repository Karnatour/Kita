#pragma once
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API ColorUtil {
    public:
        static glm::vec3 srgbToLinear(const glm::vec3& c);
    private:
        static constexpr float srgbToLinear(float c);
    };
} // Kita
