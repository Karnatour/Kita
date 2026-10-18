#include "KeyboardCallbacks.h"

#include "../../Events/EventManager.h"
#include "../../Events/KeyboardEvents.h"
#include "../../Input/InputKeys.h"

namespace Kita {
    void keyboardKeyCallbackFun(GLFWwindow* /*window*/, const int key, const int /*scancode*/, const int action, const int mods) {
        KeyPressed pressedEvent(InputKeys::convertGLFWToKitaKey(key), mods);
        KeyReleased releasedEvent(InputKeys::convertGLFWToKitaKey(key));
        switch (action) {
            case GLFW_PRESS:
                EventManager::triggerEvent(pressedEvent);
                break;
            case GLFW_RELEASE:
                EventManager::triggerEvent(releasedEvent);
                break;
            case GLFW_REPEAT:
                //Emulate REPEAT as keyPreesed event
                //EventManager::triggerEvent(pressedEvent);
                break;
            default:
                KITA_ENGINE_WARN("keyboardKeyCallbackFun recived unknown action");
                break;
        }
    }
}
