#pragma once
#include <glm/glm.hpp>

#include "../../Core/DllTemplate.h"

namespace Kita {
    struct CameraProperties {
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec4 position;
        glm::vec4 front;
    };

    class KITAENGINE_API Camera {
    public:
        virtual ~Camera() = default;
        Camera(glm::vec3 position, glm::vec3 up);
        Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float movementSpeed, float sensitivity, float zoom);
        glm::mat4 getViewMatrix() const;
        float getZoom() const;
        glm::vec3 getPosition() const;
        glm::vec3 getFront() const;
        void update();
        CameraProperties& getCameraData();

    private:
        virtual void updatePosition();
        virtual void updateCamera();
        virtual void updateMovement();
        virtual void updateZoom();

        glm::vec3 m_position;
        glm::vec3 m_front{};
        glm::vec3 m_up;
        glm::vec3 m_right{};
        glm::vec3 m_worldUp{};

        float m_yaw;
        float m_pitch;

        float m_movementSpeed;
        float m_sensitivity;
        float m_zoom;

        CameraProperties m_cameraData;
    };
} // Kita
