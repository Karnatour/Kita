#include "../../kitapch.h"
#include "Entity.h"

namespace Kita {
    Entity::Entity(Scene* scene) : m_scene(scene), m_enttEntity(m_scene->m_registry.create()) {
    }
} // Kita
