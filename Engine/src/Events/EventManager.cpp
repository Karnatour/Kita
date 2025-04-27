#include "EventManager.h"
#include "EngineListeners.h"
#include "../Input/Input.h"
#include "KeyboardEvents.h"
#include "MouseEvents.h"
#include "WindowEvents.h"

namespace Kita {
    // Define the static eventListeners map
    std::unordered_map<std::type_index, std::vector<EventManager::eventCallbackFun>> EventManager::eventListeners;

    void EventManager::attachEngineEvents() {
        listenToEvent<KeyPressed>(Input::updateKeyPress);
        listenToEvent<KeyReleased>(Input::updateKeyRelease);
        listenToEvent<MousePressed>(Input::updateButtonPress);
        listenToEvent<MouseReleased>(Input::updateButtonRelease);
        listenToEvent<WindowClosed>(EngineListeners::closeEngine);
    }

    std::vector<EventManager::eventCallbackFun>& EventManager::getCallbacks(const std::type_index type) {
        return eventListeners[type];
    }
}
