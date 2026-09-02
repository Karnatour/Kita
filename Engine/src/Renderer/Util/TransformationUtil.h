#pragma once
#include <glm/fwd.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_quat.hpp>

#include "../../Core/DllTemplate.h"

namespace Kita {
    struct KITAENGINE_API TransformationUtil {

        struct DecomposedTransform {
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;
        };

        static glm::mat4 translate(const glm::mat4& matrix, glm::vec3 moveVec);
        static glm::mat4 translateWorld(const glm::mat4& matrix, glm::vec3 moveVec);
        static glm::mat4 rotate(const glm::mat4& matrix, float angleDegree, glm::vec3 rotateVec);
        static glm::mat4 rotateWorld(const glm::mat4& matrix, float angleDegree, glm::vec3 rotateVec);
        static glm::mat4 scale(const glm::mat4& matrix, glm::vec3 scaleVec);
        static glm::vec3 getPosition(const glm::mat4& matrix);
        static glm::quat getRotation(const glm::mat4& matrix);
        static glm::vec3 getScale(const glm::mat4& matrix);
        static DecomposedTransform decompose(const glm::mat4& matrix);
    };
} // Kita
