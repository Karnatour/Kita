#pragma once
#include "../../Core/DllTemplate.h"
#include "../Buffers/FrameBuffer.h"
#include <glm/glm.hpp>

namespace Kita {
    struct CameraProperties;

    enum class LightType {
        DIRECTIONAL = 0,
        POINT = 1,
        SPOT = 2
    };

    struct KITAENGINE_API LightUtil {
        //16 because of std430 rules;
        static constexpr int MAX_LIGHTS = 512;

        static std::vector<glm::vec4> getFrustrumPoints(const glm::mat4& view, const glm::mat4& projection);
        static glm::mat4 getLightSpaceMatrix(const CameraProperties& properties, float zNear, float zFar, std::pair<float, float> resolution);
    };
} // Kita
