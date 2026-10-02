#include "CameraUtil.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../../Core/Engine.h"
#include "../Scene/ECS/Components/CameraComponent.h"

namespace Kita {
    glm::mat4 CameraUtil::getViewMatrix(const glm::vec3 position, const glm::vec3 front, const glm::vec3 up) {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 CameraUtil::getViewMatrix(const CameraProperties& properties) {
        return getViewMatrix(properties.position, properties.front, properties.up);
    }

    glm::mat4 CameraUtil::getProjectionMatrix(const CameraProperties& properties, const std::pair<int, int> viewport) {
        return getProjectionMatrix(properties.fov, viewport, properties.zNear, properties.zFar);
    }

    glm::mat4 CameraUtil::getProjectionMatrix(const float fov, const std::pair<int, int> viewport, const float zNear, const float zFar) {
        return glm::perspective(glm::radians(fov), static_cast<float>(viewport.first) / static_cast<float>(viewport.second), zNear, zFar);
    }

    void CameraUtil::updateOrientationVectors(CameraProperties& properties) {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(properties.yaw)) * cos(glm::radians(properties.pitch));
        newFront.y = sin(glm::radians(properties.pitch));
        newFront.z = sin(glm::radians(properties.yaw)) * cos(glm::radians(properties.pitch));
        properties.front = glm::normalize(newFront);

        properties.right = glm::normalize(glm::cross(properties.front, properties.worldUp));
        properties.up = glm::normalize(glm::cross(properties.right, properties.front));
    }

    void CameraUtil::deactivateCameras(Scene& scene) {
        for (const auto& [entityID, camera] : scene.view<CameraComponent>().each()) {
            auto entity = Entity(&scene, entityID);
            if (entity.hasAllComponents<ActiveCamera>()) {
                entity.removeComponent<ActiveCamera>();
            }
        }
    }

    bool CameraUtil::activeCameraExists(Scene& scene) {
        return std::ranges::any_of(scene.view<CameraComponent, ActiveCamera>().each(), [](const auto&) { return true; });
    }

    bool CameraUtil::isCameraActive(const Entity entity) {
        return entity.hasAllComponents<CameraComponent, ActiveCamera>();
    }

    void CameraUtil::markCameraAsActive(Entity entity) {
        if (activeCameraExists(*entity.getScene())) {
            deactivateCameras(*entity.getScene());
        }
        return entity.addComponent<ActiveCamera>();
    }
} // Kita
