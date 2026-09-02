#pragma once
#include <glm/glm.hpp>

namespace Kita {
    struct TransformationComponent {
        glm::mat4 localModel = glm::mat4(1.0f);
        glm::mat4 worldModel = glm::mat4(1.0f);
    };
} //Kita
