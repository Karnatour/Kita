#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Time.h"
#include "../Events/EventManager.h"
#include "../Input/Input.h"

namespace Kita {
    Camera::Camera(glm::vec3 position, glm::vec3 up) : m_position(position), m_up(up) {
        EventManager::listenToEvent<MouseMoved>([this](MouseMoved& event) {
            updateMovement(event);
        });
        EventManager::listenToEvent<MouseScrolled>([this](MouseScrolled& event) {
            updateZoom(event);
        });
        EventManager::listenToEvent<KeyPressed>([this](KeyPressed& event) {
            updatePosition(event);
        });

        m_worldUp = up;
        m_yaw = -90.0f;
        m_pitch = 0.0f;
        m_movementSpeed = 5.0f;
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

    void Camera::updatePosition(KeyPressed& event) {
        const float velocity = m_movementSpeed * Time::getDeltaTime();
        if (event.getKey() == KeyboardKey::KEY_W) {
            m_position = m_position + m_front * velocity;
        }
        if (event.getKey() == KeyboardKey::KEY_A) {
            m_position = m_position - m_right * velocity;
        }
        if (event.getKey() == KeyboardKey::KEY_S) {
            m_position = m_position - m_front * velocity;
        }
        if (event.getKey() == KeyboardKey::KEY_D) {
            m_position = m_position + m_right * velocity;
        }
    }

    glm::vec3 Camera::getPosition() {
        return m_position;
    }

    void Camera::updateMovement(MouseMoved& event) {
        float xpos = event.getPosition().first;
        float ypos = event.getPosition().second;

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        xoffset = xoffset * m_sensitivity;
        yoffset = yoffset * m_sensitivity;

        lastX = xpos;
        lastY = ypos;

        m_yaw = m_yaw + xoffset;
        m_pitch = m_pitch + yoffset;

        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;

        updateCamera();
    }

    void Camera::updateZoom(MouseScrolled& event) {
        m_zoom = m_zoom - event.getOffset().second;

        if (m_zoom < 1.0f)
            m_zoom = 1.0f;
        if (m_zoom > 45.0f)
            m_zoom = 45.0f;
    }
} // Kita
