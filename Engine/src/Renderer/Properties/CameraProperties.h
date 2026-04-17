#pragma once
#include <glm/glm.hpp>

namespace Kita {
    struct CameraProperties {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 front = glm::vec3(0.f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);;

        float yaw = 0.0f;
        float pitch = 0.0f;

        float movementSpeed = 10.0f;
        float sensitivity = 0.1f;
        float zoom = 45.0f;

        float fov = 55.0f;
        float zNear = 0.1f;
        float zFar = 1000.0f;
    };
}
