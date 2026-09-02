#include "../../kitapch.h"
#include "TransformationUtil.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Kita {
    glm::mat4 TransformationUtil::translate(const glm::mat4& matrix, const glm::vec3 moveVec) {
        return glm::translate(matrix, moveVec);
    }

    glm::mat4 TransformationUtil::translateWorld(const glm::mat4& matrix, const glm::vec3 moveVec) {
        return glm::translate(glm::mat4(1.0f), moveVec) * matrix;
    }

    glm::mat4 TransformationUtil::rotate(const glm::mat4& matrix, const float angleDegree, const glm::vec3 rotateVec) {
        return glm::rotate(matrix, glm::radians(angleDegree), rotateVec);
    }

    glm::mat4 TransformationUtil::rotateWorld(const glm::mat4& matrix, const float angleDegree, const glm::vec3 rotateVec) {
        return glm::rotate(glm::mat4(1.0f), glm::radians(angleDegree), rotateVec) * matrix;
    }

    glm::mat4 TransformationUtil::scale(const glm::mat4& matrix, const glm::vec3 scaleVec) {
        return glm::scale(matrix, scaleVec);
    }

    glm::vec3 TransformationUtil::getPosition(const glm::mat4& matrix) {
        return glm::vec3(matrix[3]);
    }

    glm::quat TransformationUtil::getRotation(const glm::mat4& matrix) {
        const glm::vec3 col0 = glm::normalize(glm::vec3(matrix[0]));
        const glm::vec3 col1 = glm::normalize(glm::vec3(matrix[1]));
        const glm::vec3 col2 = glm::normalize(glm::vec3(matrix[2]));
        return glm::quat_cast(glm::mat3(col0, col1, col2));
    }

    glm::vec3 TransformationUtil::getScale(const glm::mat4& matrix) {
        return glm::vec3(glm::length(glm::vec3(matrix[0])), glm::length(glm::vec3(matrix[1])), glm::length(glm::vec3(matrix[2])));
    }

    TransformationUtil::DecomposedTransform TransformationUtil::decompose(const glm::mat4& matrix) {
        DecomposedTransform result;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(matrix, result.scale, result.rotation, result.position, skew, perspective);
        return result;
    }
} // Kita
