#pragma once
#include <glm/glm.hpp>

#include "../Events/MouseEvents.h"
#include "../Core/DllTemplate.h"
#include "../Events/KeyboardEvents.h"

namespace Kita {

class KITAENGINE_API Camera {
public:
    Camera(glm::vec3 position, glm::vec3 up);
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float movementSpeed, float sensitivity, float zoom);
    glm::mat4 getViewMatrix() const;
    float getZoom();
    glm::vec3 getPosition();
private:
    void updatePosition(KeyPressed& event);
    void updateCamera();
    void updateMovement(MouseMoved& event);
    void updateZoom(MouseScrolled& event);

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

    bool firstMouse = true;
    float lastX,lastY;
};

} // Kita
