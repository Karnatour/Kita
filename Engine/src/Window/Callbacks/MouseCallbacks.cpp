#include "MouseCallbacks.h"

#include "../../Events/EventManager.h"
#include "../../Events/MouseEvents.h"

namespace Kita {
    void mouseButtonCallbackFun(GLFWwindow* window, int button, int action, int mods) {
        MousePressed pressedEvent(convertGLFWToKitaButton(button), mods);
        MouseReleased releasedEvent(convertGLFWToKitaButton(button));
        switch (action) {
        case GLFW_PRESS:
            EventManager::triggerEvent(pressedEvent);
            break;
        case GLFW_RELEASE:
            EventManager::triggerEvent(releasedEvent);
            break;
        case GLFW_REPEAT:
            //Emulate REPEAT as buttonPreesed event
            //GLFW doesnt seem to call GLFW_REPEAT
            EventManager::triggerEvent(pressedEvent);
            break;
        default:
            KITA_ENGINE_WARN("mouseButtonCallbackFun recived unknown action");
            break;
        }
    }

    void cursorPosCallbackFun(GLFWwindow* window, double xpos, double ypos) {
        MouseMoved event(xpos, ypos);
        EventManager::triggerEvent(event);
    }

    void cursorEnterCallbackFun(GLFWwindow* window, int entered) {
        MouseChangedFocus event(entered);
        EventManager::triggerEvent(event);
    }

    void scrollCallbackFun(GLFWwindow* window, double xoffset, double yoffset) {
        MouseScrolled event(xoffset, yoffset);
        EventManager::triggerEvent(event);
    }
}
