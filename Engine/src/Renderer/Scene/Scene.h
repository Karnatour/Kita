#pragma once
#include <entt/entity/registry.hpp>
#include "../../Core/DllTemplate.h"
#include "ECS/Systems/System.h"

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
            m_systems.emplace_back(std::make_unique<T>());
        }

        template <typename... Components>
        auto view() {
            return m_registry.view<Components...>();
        }

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
    };
} // Kita
