#pragma once
#include <glm/glm.hpp>

#include "../Core/DllTemplate.h"

namespace Kita {

class KITAENGINE_API Camera {
public:
    virtual ~Camera() = default;
    Camera(glm::vec3 position, glm::vec3 up);
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float movementSpeed, float sensitivity, float zoom);
    glm::mat4 getViewMatrix() const;
    float getZoom();
    glm::vec3 getPosition();
    void update();
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

};

} // Kita
