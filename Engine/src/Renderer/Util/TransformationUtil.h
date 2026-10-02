#pragma once
#include <glm/fwd.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_quat.hpp>

#include "../../Core/DllTemplate.h"

namespace Kita {
    class Entity;

    struct KITAENGINE_API TransformationUtil {
        struct DecomposedTransform {
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;
        };

        static glm::mat4 translate(const glm::mat4& matrix, glm::vec3 move);
        static glm::mat4 translateWorld(const glm::mat4& matrix, glm::vec3 move);
        static glm::mat4 rotate(const glm::mat4& matrix, float angleDegree, glm::vec3 rotate);
        static glm::mat4 rotateWorld(const glm::mat4& matrix, float angleDegree, glm::vec3 rotate);
        static glm::mat4 scaleWorld(const glm::mat4& matrix, glm::vec3 scale);
        static glm::mat4 scale(const glm::mat4& matrix, glm::vec3 scale);
        static glm::mat4 setPosition(const glm::mat4& matrix, glm::vec3 position);
        static glm::vec3 getPosition(const glm::mat4& matrix);
        static glm::quat getRotation(const glm::mat4& matrix);
        static glm::vec3 getScale(const glm::mat4& matrix);
        static DecomposedTransform decompose(const glm::mat4& matrix);
        static void setWorldPosition(Entity entity, glm::vec3 position);
    private:
        static void updateChildrenWorld(Entity entity);
    };
} // Kita
