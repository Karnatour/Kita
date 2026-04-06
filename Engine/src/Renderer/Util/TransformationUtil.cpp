#include "../../kitapch.h"
#include "TransformationUtil.h"

#include <glm/ext/matrix_transform.hpp>

namespace Kita {
    glm::mat4 TransformationUtil::translate(const glm::mat4& matrix, const glm::vec3& moveVec) {
        return glm::translate(matrix, moveVec);
    }

    glm::mat4 TransformationUtil::rotate(const glm::mat4& matrix, const float angleDegree, const glm::vec3& rotateVec) {
        return glm::rotate(matrix, glm::radians(angleDegree), rotateVec);
    }

    glm::mat4 TransformationUtil::scale(const glm::mat4& matrix, const glm::vec3& scaleVec) {
        return glm::scale(matrix, scaleVec);
    }
} // Kita
