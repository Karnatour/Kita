#include "../../kitapch.h"
#include "Transformation.h"

#include <glm/ext/matrix_transform.hpp>

namespace Kita {
    glm::mat4 Transformation::getModelMatrix() const {
        return m_model;
    }

    void Transformation::setModelMatrix(const glm::mat4& model) {
        m_model = model;
    }

    void Transformation::translate(const glm::vec3& moveVec) {
        m_model = glm::translate(m_model, moveVec);
    }

    void Transformation::scale(const glm::vec3& scaleVec) {
        m_model = glm::scale(m_model, scaleVec);
    }
} // Kita
