#pragma once
#include "../../Core/DllTemplate.h"
#include "../Buffers/FrameBuffer.h"
#include <glm/glm.hpp>

namespace Kita {
    struct CameraProperties;

    enum class LightType : int32_t {
        DIRECTIONAL = 0,
        POINT = 1,
        SPOT = 2
    };

    struct KITAENGINE_API LightUtil {
        static std::vector<glm::vec4> getFrustrumPoints(const glm::mat4& view, const glm::mat4& projection);
        static glm::mat4 getLightSpaceMatrix(const CameraProperties& properties, float zNear, float zFar, const glm::vec3& lightDir, std::pair<int, int> viewportResolution, std::pair<int, int> shadowMapResolution);
    };
} // Kita
