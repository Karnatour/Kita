#pragma once
#include <entt/entity/registry.hpp>

#include "Camera.h"
#include "../../Core/DllTemplate.h"

namespace Kita {
    class Entity;

    class KITAENGINE_API Scene {
    public:
        Scene();
        void update();
        void render() const;
        Entity createEntity();
        Camera& getCamera();
        Entity* getEntityByID(unsigned int id) const;

    private:
        friend class Entity;

        entt::registry m_registry;

    };
} // Kita
