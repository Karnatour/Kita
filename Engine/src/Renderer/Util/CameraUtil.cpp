#include "../../kitapch.h"
#include "CameraUtil.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../../Core/Engine.h"
#include "../../Core/Time.h"
#include "../../Input/Input.h"
#include "../../Window/Window.h"

namespace Kita {
    glm::mat4 CameraUtil::getViewMatrix(const glm::vec3 position,const glm::vec3 front,const glm::vec3 up) {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 CameraUtil::getProjectionMatrix(const float fov, const float width, const float height, const float zNear, const float zFar) {
        return glm::perspective(glm::radians(fov), width / height, zNear, zFar);
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

    void CameraUtil::updatePosition(CameraProperties& properties) {
        glm::vec3 direction(0.0f);

        if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_W)) {
            direction += properties.front;
        }
        if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_A)) {
            direction -= properties.right;
        }
        if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_S)) {
            direction -= properties.front;
        }
        if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_D)) {
            direction += properties.right;
        }

        if (glm::length(direction) > 0.0f) {
            direction = glm::normalize(direction);
            const float velocity = properties.movementSpeed * static_cast<float>(Time::getDeltaTime());
            properties.position += direction * velocity;
        }
    }

    void CameraUtil::update(CameraProperties& properties) {
        updatePosition(properties);
        updateEulerAngles(properties);
        updateZoom(properties);
    }

    void CameraUtil::updateEulerAngles(CameraProperties& properties) {
        auto& engineWindow = Engine::getEngine()->getWindow();

        if (Input::isMousePressed(InputKeys::MouseButton::MBUTTON_RIGHT)) {
            engineWindow.setCursorMode(CursorMode::DISABLED);

            if (Input::wasMouseMoved()) {
                auto [xOffset, yOffset] = Input::getMousePos().mouseOffset;
                xOffset *= properties.sensitivity;
                yOffset *= properties.sensitivity;

                properties.yaw += static_cast<float>(xOffset);
                properties.pitch -= static_cast<float>(yOffset);

                properties.pitch = std::clamp(properties.pitch, -89.0f, 89.0f);

                updateOrientationVectors(properties);
            }
        }
        else {
            engineWindow.setCursorMode(CursorMode::SHOWN);
        }
    }

    void CameraUtil::updateZoom(CameraProperties& properties) {
        if (Input::wasMouseScrolled()) {
            properties.zoom = properties.zoom - static_cast<float>(Input::getMouseScroll().mouseScrollOffset.second);

            properties.zoom = std::clamp(properties.zoom, 1.0f, 45.0f);
        }
    }
} // Kita
