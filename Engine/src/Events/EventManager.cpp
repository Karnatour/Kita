#include "EventManager.h"
#include "EngineListeners.h"
#include "../Input/Input.h"
#include "KeyboardEvents.h"
#include "MouseEvents.h"
#include "WindowEvents.h"

namespace Kita {
    void EventManager::attachEngineEvents() {
        setInputListeners();
        listenToEvent<WindowClosed>(EngineListeners::closeEngine);
    }

    std::vector<EventManager::eventCallbackFun>& EventManager::getCallbacks(const std::type_index type) {
        return eventListeners[type];
    }

    void EventManager::setInputListeners() {
        listenToEvent<KeyPressed>(Input::updateKeyPress);
        listenToEvent<KeyReleased>(Input::updateKeyRelease);
        listenToEvent<MousePressed>(Input::updateButtonPress);
        listenToEvent<MouseReleased>(Input::updateButtonRelease);
        listenToEvent<MouseMoved>(Input::updateMouseMovement);
        listenToEvent<MouseScrolled>(Input::updateMouseScroll);
    }
}
