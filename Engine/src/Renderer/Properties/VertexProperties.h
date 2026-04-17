#pragma once
#include <glm/glm.hpp>
#include "../../Core/DllTemplate.h"

namespace Kita {
    struct VertexProperties {
        glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec2 texture = glm::vec3(0.0f,0.0f,0.0f);
        glm::vec3 normal = glm::vec3(0.0f,0.0f,0.0f);
    };
}
