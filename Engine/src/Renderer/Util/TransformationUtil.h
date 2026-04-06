#pragma once
#include <glm/fwd.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "../../Core/DllTemplate.h"

namespace Kita {
    struct KITAENGINE_API TransformationUtil {
        static glm::mat4 translate(const glm::mat4& matrix, const glm::vec3& moveVec);
        static glm::mat4 rotate(const glm::mat4& matrix, float angleDegree, const glm::vec3& rotateVec);
        static glm::mat4 scale(const glm::mat4& matrix, const glm::vec3& scaleVec);
    };
} // Kita
