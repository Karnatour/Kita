#pragma once
#include <glm/glm.hpp>

namespace Kita {
    enum class FogMode {
        LINEAR,
        EXPONENTIAL,
        EXPONENTIAL2
    };

    struct FogProperties {
        glm::vec3 fogColor = glm::vec3(1.0f);
        float fogStart = 50.0f;
        float fogEnd = 150.0f;
        float fogDensity = 2.5f;
        FogMode fogMode = FogMode::EXPONENTIAL2;
    };

    struct EnvironmentProperties {
        FogProperties fogProperties;
    };
}
