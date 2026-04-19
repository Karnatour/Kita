#pragma once
#include "../../../Core/DllTemplate.h"
#include "../../Buffers/FrameBuffer.h"
#include <glm/glm.hpp>

namespace Kita {
    struct CameraProperties;

    enum class LightType {
        DIRECTIONAL = 0,
        POINT = 1,
        SPOT = 2
    };

    struct ShadowProperties {
        static constexpr std::pair<int, int> resolution = std::make_pair(4096, 4096);
        std::unique_ptr<FrameBuffer> depthMapFBO = FrameBuffer::createPtr();
    };

    class KITAENGINE_API LightUtil {
    public:
        //16 because of std430 rules;
        static constexpr int MAX_LIGHTS = 512;

        static std::vector<glm::vec4> getFrustrumPoints(const glm::mat4& view, const glm::mat4& projection);
        static glm::mat4 getLightSpaceMatrix(const CameraProperties& properties, float zNear, float zFar, std::pair<float, float> resolution);
    };
} // Kita
