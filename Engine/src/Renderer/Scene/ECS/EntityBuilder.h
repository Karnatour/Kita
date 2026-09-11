#pragma once

#include "Entity.h"
#include "../../../Core/DllTemplate.h"

namespace Kita {
    struct KITAENGINE_API EntityBuilder {
        static Entity createDirectionalLight(Scene& scene, bool castShadows = true);
        static Entity createPlayerCharacter(Scene& scene, const std::optional<std::string>& name, glm::vec3 position = glm::vec3(0.0f));
        static Entity createActiveCamera(Scene& scene);
    };
} // Kita
