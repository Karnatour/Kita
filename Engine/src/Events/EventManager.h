#pragma once
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <functional>
#include "Event.h"

namespace Kita {
    class KITAENGINE_API EventManager {
    public:
        static void attachEngineEvents();
        using eventCallbackFun = std::function<void(Event&)>;

        template <typename T>
        static void listenToEvent(std::function<void(T&)> listenerFun) {
            auto& callbacks = getCallbacks(typeid(T));
            callbacks.push_back([listenerFun](Event& event) {
                listenerFun(static_cast<T&>(event)); // Downcast Event to T
            });
        }

        template <typename T>
        static void triggerEvent(T& event) {
            auto& callbacks = getCallbacks(typeid(T));
            //KITA_ENGINE_DEBUG("Event triggered: {} | Callbacks count: {}", typeid(T).name(), callbacks.size());

            for (auto& function : callbacks) {
                function(event);
            }
        }

    private:
        static std::vector<eventCallbackFun>& getCallbacks(std::type_index type);

        static std::unordered_map<std::type_index, std::vector<eventCallbackFun>> eventListeners;
    };
}