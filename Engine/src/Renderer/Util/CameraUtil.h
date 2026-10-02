#pragma once
#include <entt/entity/fwd.hpp>
#include <glm/glm.hpp>

#include "../../Core/DllTemplate.h"
#include "../Properties/CameraProperties.h"

namespace Kita {
    class Scene;
    class Entity;

    struct KITAENGINE_API CameraUtil {
        static glm::mat4 getViewMatrix(glm::vec3 position, glm::vec3 front, glm::vec3 up);
        static glm::mat4 getViewMatrix(const CameraProperties& properties);
        static glm::mat4 getProjectionMatrix(const CameraProperties& properties, std::pair<int, int> viewport);
        static glm::mat4 getProjectionMatrix(float fov, std::pair<int, int> viewport, float zNear, float zFar);
        static void updateOrientationVectors(CameraProperties& properties);

        //Removes ActiveCamera component from all cameras;
        static void deactivateCameras(Scene& scene);

        static bool activeCameraExists(Scene& scene);
        static bool isCameraActive(Entity entity);
        static void markCameraAsActive(Entity entity);
    };
} // Kita
