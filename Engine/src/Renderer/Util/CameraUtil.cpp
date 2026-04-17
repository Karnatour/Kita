#include "../../kitapch.h"
#include "CameraUtil.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../../Core/Engine.h"

namespace Kita {
    glm::mat4 CameraUtil::getViewMatrix(const glm::vec3 position, const glm::vec3 front, const glm::vec3 up) {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 CameraUtil::getViewMatrix(const CameraProperties& properties) {
        return getViewMatrix(properties.position, properties.front, properties.up);
    }

    glm::mat4 CameraUtil::getProjectionMatrix(const CameraProperties& properties, const std::pair<float, float> viewport) {
        return getProjectionMatrix(properties.fov, viewport, properties.zNear, properties.zFar);
    }

    glm::mat4 CameraUtil::getProjectionMatrix(const float fov, const std::pair<float, float> viewport, const float zNear, const float zFar) {
        return glm::perspective(glm::radians(fov), viewport.first / viewport.second, zNear, zFar);
    }
} // Kita
