#pragma once
#include <entt/entity/registry.hpp>

#include "../Util/CameraUtil.h"
#include "../../Core/DllTemplate.h"
#include "../Systems/CameraSystem.h"

namespace Kita {
    class Entity;

    class KITAENGINE_API Scene {
    public:
        Scene();
        void update();
        void render() const;
        Entity createEntity();
        std::optional<Entity> getEntityByID(unsigned int id) const;

        template<typename... Components>
        auto view() {
            return m_registry.view<Components...>();
        }

    private:
        friend class Entity;

        entt::registry m_registry;
        CameraSystem m_cameraSystem;

    };
} // Kita
