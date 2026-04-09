#pragma once
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <functional>
#include "Event.h"

namespace Kita {
    class KITAENGINE_API EventManager {
    public:
        using eventCallbackFun = std::function<void(Event&)>;

        template <std::derived_from<Event> T>
        static void listenToEvent(std::function<void(T&)> listenerFun) {
            auto& callbacks = getCallbacks(typeid(T));
            callbacks.push_back([listenerFun](Event& event) {
                return listenerFun(static_cast<T&>(event)); // Downcast Event to T
            });
        }

        template <std::derived_from<Event> T>
        static void triggerEvent(T& event) {
            auto& callbacks = getCallbacks(typeid(T));
            //KITA_ENGINE_DEBUG("Event triggered: {} | Callbacks count: {}", typeid(T).name(), callbacks.size());

            for (auto& function : callbacks) {
                function(event);
            }
        }

    private:
        static std::vector<eventCallbackFun>& getCallbacks(std::type_index type);

        static inline std::unordered_map<std::type_index, std::vector<eventCallbackFun>> m_eventListeners;
    };
}