#pragma once
#include "Scene.h"
#include "entt/entt.hpp"
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Entity {
    public:
        Entity() = default;
        explicit Entity(Scene* scene);
        Entity(Scene* scene, entt::entity entityID);

        template <typename... Ts>
        bool hasAllComponents() const {
            return m_scene->m_registry.all_of<Ts...>(m_enttEntity);
        }

        template <typename... Ts>
        bool hasAnyComponent() const {
            return m_scene->m_registry.any_of<Ts...>(m_enttEntity);
        }

        template <typename T, typename... Args>
        void addComponent(Args&&... arg) {
            m_scene->m_registry.emplace<T>(m_enttEntity, std::forward<Args>(arg)...);
        }

        template <typename T>
        void removeComponent() const {
            m_scene->m_registry.remove<T>(m_enttEntity);
        }

        template <typename T>
        T& getComponent() {
            return m_scene->m_registry.get<T>(m_enttEntity);
        }

    private:
        Scene* m_scene = nullptr;
        entt::entity m_enttEntity = entt::null;
    };
} // Kita
