#pragma once
#include <entt/entity/registry.hpp>
#include "../../Core/DllTemplate.h"
#include "ECS/Systems/System.h"
#include <vector>
#include <typeindex>
#include <unordered_map>

namespace Kita {
    class Entity;

    class KITAENGINE_API Scene {
    public:
        void addDefaultSystems();
        void update();
        void render();
        Entity createEntity();


        template <std::derived_from<System> T>
        void addSystem() {
            auto system = std::make_unique<T>(*this);
            m_systemLookup[std::type_index(typeid(T))] = system.get();
            m_systems.emplace_back(std::move(system));
        }

        template <std::derived_from<System> T>
        T* getSystem() {
            const auto it = m_systemLookup.find(std::type_index(typeid(T)));
            if (it == m_systemLookup.end()) {
                return nullptr;
            }
            return static_cast<T*>(it->second);
        }

        template <typename... Components>
        auto view() {
            return m_registry.view<Components...>();
        }

        //Stored as entt entity, so we don't get circular dependency
        entt::entity getCameraEntity() const;

    private:
        friend class Entity;

        entt::registry m_registry;
        /*
         *  Binding points: TODO: Store/Manage the binding points somewhere
         *  CameraUBO = 0
         *  DirLightUBO = 1
         *  PhongPropertiesUBO = 2
         *  LightsSSBO = 0
        */
        std::vector<std::unique_ptr<System>> m_systems;
        std::unordered_map<std::type_index, System*> m_systemLookup;
        entt::entity m_camera;
    };
} // Kita
