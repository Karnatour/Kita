#pragma once
#include <glm/vec3.hpp>

namespace Kita {
    struct PhongProperties {
        glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
        float shininess = 0.0f;
    };
}
