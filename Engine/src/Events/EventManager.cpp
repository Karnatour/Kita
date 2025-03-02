#include "EventManager.h"
#include "../Input.h"
#include "KeyboardEvents.h"

namespace Kita {
    // Define the static eventListeners map
    std::unordered_map<std::type_index, std::vector<EventManager::eventCallbackFun>> EventManager::eventListeners;

    void EventManager::attachEngineEvents() {
        listenToEvent<KeyPressed>(Input::updateKey);
    }

    std::vector<EventManager::eventCallbackFun>& EventManager::getCallbacks(const std::type_index type) {
        return eventListeners[type];
    }
}
