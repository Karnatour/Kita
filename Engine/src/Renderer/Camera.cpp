#include "../kitapch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Engine.h"
#include "../Core/Time.h"
#include "../Input/Input.h"
#include "../Window/Window.h"

namespace Kita {
    Camera::Camera(glm::vec3 position, glm::vec3 up) : m_position(position), m_up(up) {
        m_worldUp = up;
        m_yaw = -90.0f;
        m_pitch = 0.0f;
        m_movementSpeed = 10.0f;
        m_sensitivity = 0.1f;
        m_zoom = 45.0f;
        updateCamera();
    }

    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float movementSpeed, float sensitivity, float zoom) {
        m_worldUp = up;
        m_position = position;
        m_up = up;
        m_yaw = yaw;
        m_pitch = pitch;
        m_movementSpeed = movementSpeed;
        m_sensitivity = sensitivity;
        m_zoom = zoom;
        updateCamera();
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    float Camera::getZoom() {
        return m_zoom;
    }

    void Camera::updateCamera() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        newFront.y = sin(glm::radians(m_pitch));
        newFront.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(newFront);

        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

    void Camera::updatePosition() {
        const float velocity = m_movementSpeed * static_cast<float>(Time::getDeltaTime());
        if (Input::isKeyPressed(KeyboardKey::KEY_W)) {
            m_position = m_position + m_front * velocity;
        }
        if (Input::isKeyPressed(KeyboardKey::KEY_A)) {
            m_position = m_position - m_right * velocity;
        }
        if (Input::isKeyPressed(KeyboardKey::KEY_S)) {
            m_position = m_position - m_front * velocity;
        }
        if (Input::isKeyPressed(KeyboardKey::KEY_D)) {
            m_position = m_position + m_right * velocity;
        }
    }

    glm::vec3 Camera::getPosition() {
        return m_position;
    }

    void Camera::update() {
        updatePosition();
        updateMovement();
        updateZoom();
    }

    CameraMatrices& Camera::getCameraMatrices() {
        return m_matrices;
    }

    void Camera::updateMovement() {
        auto& engineWindow = Engine::getEngine()->getWindow();

        if (Input::isMousePressed(MouseButton::MBUTTON_RIGHT)) {
            engineWindow.setCursorMode(CursorMode::DISABLED);

            if (Input::wasMouseMoved()) {
                auto offset = Input::getMousePos().mouseOffset;
                offset.first *= m_sensitivity;
                offset.second *= m_sensitivity;

                m_yaw += static_cast<float>(offset.first);
                m_pitch -= static_cast<float>(offset.second);

                m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

                updateCamera();
            }
        }
        else {
            engineWindow.setCursorMode(CursorMode::SHOWN);
        }
    }

    void Camera::updateZoom() {
        if (Input::wasMouseScrolled()) {
            m_zoom = m_zoom - static_cast<float>(Input::getMouseScroll().mouseScrollOffset.second);

            m_zoom = std::clamp(m_zoom, 1.0f, 45.0f);
        }
    }
} // Kita
