#pragma once
#include <glm/glm.hpp>
#include "../../Core/DllTemplate.h"

namespace Kita {
    struct KITAENGINE_API Vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texture;
    };
}
