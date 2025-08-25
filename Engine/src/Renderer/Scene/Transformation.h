#pragma once
#include <glm/glm.hpp>

#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Transformation {
    public:
        glm::mat4 getModelMatrix() const;
        void setModelMatrix(const glm::mat4& model);
        void translate(const glm::vec3& moveVec);
        void scale(const glm::vec3& scaleVec);
    private:
        glm::mat4 m_model = glm::mat4(1.0f);
    };
} // Kita