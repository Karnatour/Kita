#pragma once
#include <glm/glm.hpp>

namespace Kita {
    struct TransformationComponent {
        glm::mat4 model = glm::mat4(1.0f);
    };
} //Kita
