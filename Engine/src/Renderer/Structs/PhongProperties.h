#pragma once
#include <glm/vec3.hpp>
#include "../../Core/DllTemplate.h"

namespace Kita {
    struct KITAENGINE_API PhongProperties {
        glm::vec3 ambient = {0.0f, 0.0f, 0.0f};
        glm::vec3 diffuse = {0.0f, 0.0f, 0.0f};
        glm::vec3 specular = {0.0f, 0.0f, 0.0f};
        float shininess = 0.0f;
    };
}
