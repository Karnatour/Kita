#include "../../kitapch.h"
#include "Entity.h"

namespace Kita {
    Entity::Entity(Scene* scene) : m_scene(scene), m_enttEntity(m_scene->m_registry.create()) {
    }

    Entity::Entity(Scene* scene, const entt::entity entityID) : m_scene(scene), m_enttEntity(entityID) {
    }
} // Kita
