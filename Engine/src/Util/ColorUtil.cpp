#include "ColorUtil.h"

namespace Kita {
    constexpr float ColorUtil::srgbToLinear(const float c) {
        return c <= 0.04045f ? c / 12.92f : std::pow((c + 0.055f) / 1.055f, 2.4f);
    }

    glm::vec3 ColorUtil::srgbToLinear(const glm::vec3& c) {
        return glm::vec3(srgbToLinear(c.r), srgbToLinear(c.g), srgbToLinear(c.b));
    }
} // Kita
