#include "EventManager.h"

namespace Kita {
    std::vector<EventManager::eventCallbackFun>& EventManager::getCallbacks(const std::type_index type) {
        return m_eventListeners[type];
    }
}
