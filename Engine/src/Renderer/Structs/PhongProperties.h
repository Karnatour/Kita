#pragma once
#include <glm/vec4.hpp>
#include "../../Core/DllTemplate.h"

namespace Kita {
    struct KITAENGINE_API PhongProperties {
        glm::vec4 ambient = {0.0f, 0.0f, 0.0f,1.0f};
        glm::vec4 diffuse = {0.0f, 0.0f, 0.0f,1.0f};
        glm::vec4 specular = {0.0f, 0.0f, 0.0f,1.0f};
        float shininess = 0.0f;
        float pad1,pad2,pad3;
    };
}
