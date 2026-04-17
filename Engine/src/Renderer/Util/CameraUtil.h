#pragma once
#include <glm/glm.hpp>

#include "../../Core/DllTemplate.h"
#include "../Properties/CameraProperties.h"

namespace Kita {
    struct KITAENGINE_API CameraUtil {
        static glm::mat4 getViewMatrix(glm::vec3 position, glm::vec3 front, glm::vec3 up);
        static glm::mat4 getViewMatrix(const CameraProperties& properties);
        static glm::mat4 getProjectionMatrix(const CameraProperties& properties, std::pair<float, float> viewport);
        static glm::mat4 getProjectionMatrix(float fov, std::pair<float, float> viewport, float zNear, float zFar);
    };
} // Kita
