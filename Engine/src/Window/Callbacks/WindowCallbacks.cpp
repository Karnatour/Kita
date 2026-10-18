#include "WindowCallbacks.h"

#include "../../Events/EventManager.h"
#include "../../Events/WindowEvents.h"

namespace Kita {
    void windowPosCallbackFun(GLFWwindow* /*window*/, const int xpos, const int ypos) {
        WindowMoved event(xpos, ypos);
        EventManager::triggerEvent(event);
    }

    void windowSizeCallbackFun(GLFWwindow* /*window*/, const int width, const int height) {
        WindowResized event(width, height);
        EventManager::triggerEvent(event);
    }

    void windowCloseCallbackFun(GLFWwindow* /*window*/) {
        WindowClosed event;
        EventManager::triggerEvent(event);
    }

    void windowFocusCallbackFun(GLFWwindow* /*window*/, const int focused) {
        WindowChangedFocus event(focused);
        EventManager::triggerEvent(event);
    }

    void frameBufferSizeCallbackFun(GLFWwindow* /*window*/, const int width, const int height) {
        FrameBufferResized event(width,height);
        EventManager::triggerEvent(event);
    }
}
