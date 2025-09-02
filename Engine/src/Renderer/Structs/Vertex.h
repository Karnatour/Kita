#pragma once
#include <glm/glm.hpp>
#include "../../Core/DllTemplate.h"

namespace Kita {
    struct KITAENGINE_API Vertex {
        glm::vec3 position{};
        glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec2 texture{};
        glm::vec3 normal{};
    };
}
