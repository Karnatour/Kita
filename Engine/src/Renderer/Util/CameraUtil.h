#pragma once
#include <glm/glm.hpp>

#include "../../Core/DllTemplate.h"

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
    };

    class KITAENGINE_API CameraUtil {
    public:
        static glm::mat4 getViewMatrix(glm::vec3 position, glm::vec3 front, glm::vec3 up);
        static glm::mat4 getProjectionMatrix(float fov, float width, float height, float zNear, float zFar);
        static void update(CameraProperties& properties);

    private:
        static void updatePosition(CameraProperties&);
        static void updateOrientationVectors(CameraProperties& properties);
        static void updateEulerAngles(CameraProperties& properties);
        static void updateZoom(CameraProperties& properties);
    };
} // Kita
