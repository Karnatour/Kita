#pragma once
#include "../Transformation.h"

namespace Kita {
    struct TransformationComponent {
        glm::mat4 m_model = glm::mat4(1.0f);
    };
} //Kita
