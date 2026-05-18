#include "../../../../kitapch.h"
#include "../../../../Core/Time.h"
#include "../../../../Core/Engine.h"
#include "../../../../Input/Input.h"
#include "../Components/CameraComponent.h"
#include "CameraSystem.h"

namespace Kita {
    int CameraSystem::getOrder() {
        return Order::CAMERA;
    }

    void CameraSystem::update(Scene& scene) {
        for (auto [entity,camera] : scene.view<CameraComponent>().each()) {
            CameraProperties& properties = camera.properties;

            updatePosition(properties);
            updateEulerAngles(properties);
            updateZoom(properties);
        }
    }

    void CameraSystem::render(Scene& scene) {
        // lazy creation on render thread
        if (m_cameraUBO == nullptr) {
            m_cameraUBO = UniformBuffer::createPtr();
            m_cameraUBO->createBuffer(sizeof(CameraUBOLayout), &m_activeCameraData);
        }

        const auto enttEntity = scene.view<CameraComponent, ActiveCamera>().front();
        if (enttEntity == entt::null) {
            return;
        }

        auto activeCamera = Entity(&scene, enttEntity);
        updateActiveCameraData(activeCamera.getComponent<CameraComponent>().properties);

        m_cameraUBO->bind(0);
        m_cameraUBO->upload(sizeof(CameraUBOLayout), &m_activeCameraData);
    }

    void CameraSystem::updateOrientationVectors(CameraProperties& properties) {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(properties.yaw)) * cos(glm::radians(properties.pitch));
        newFront.y = sin(glm::radians(properties.pitch));
        newFront.z = sin(glm::radians(properties.yaw)) * cos(glm::radians(properties.pitch));
        properties.front = glm::normalize(newFront);

        properties.right = glm::normalize(glm::cross(properties.front, properties.worldUp));
        properties.up = glm::normalize(glm::cross(properties.right, properties.front));
    }

    void CameraSystem::updatePosition(CameraProperties& properties) {
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

    void CameraSystem::updateActiveCameraData(const CameraProperties& properties) {
        m_activeCameraData.view = CameraUtil::getViewMatrix(properties);
        m_activeCameraData.projection = CameraUtil::getProjectionMatrix(properties, Engine::getEngine()->getRenderer().getViewport());
        m_activeCameraData.position = glm::vec4(properties.position, 1.0f);
        m_activeCameraData.front = glm::vec4(properties.front, 1.0f);
        m_activeCameraData.params.x = properties.zNear;
        m_activeCameraData.params.y = properties.zFar;
    }

    void CameraSystem::updateEulerAngles(CameraProperties& properties) {
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

    void CameraSystem::updateZoom(CameraProperties& properties) {
        if (Input::wasMouseScrolled()) {
            properties.fov = properties.fov + static_cast<float>(Input::getMouseScroll().mouseScrollOffset.second);
            properties.fov = std::clamp(properties.fov, 1.0f, 45.0f);
        }
    }
} // Kita
