#include "WindowCallbacks.h"

#include "../../Events/EventManager.h"
#include "../../Events/WindowEvents.h"

namespace Kita {
    void windowPosCallbackFun(GLFWwindow* window, int xpos, int ypos) {
        WindowMoved event(xpos, ypos);
        EventManager::triggerEvent(event);
    }

    void windowSizeCallbackFun(GLFWwindow* window, int width, int height) {
        WindowResized event(width, height);
        EventManager::triggerEvent(event);
    }

    void windowCloseCallbackFun(GLFWwindow* window) {
        WindowClosed event;
        EventManager::triggerEvent(event);
    }

    void windowFocusCallbackFun(GLFWwindow* window, int focused) {
        WindowChangedFocus event(focused);
        EventManager::triggerEvent(event);
    }
}
