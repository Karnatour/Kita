#include "TransformationUtil.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "../../Core/Engine.h"
#include "../Scene/ECS/Entity.h"
#include "../Scene/ECS/Components/Components.h"

namespace Kita {
    glm::mat4 TransformationUtil::translate(const glm::mat4& matrix, const glm::vec3 move) {
        return glm::translate(matrix, move);
    }

    glm::mat4 TransformationUtil::translateWorld(const glm::mat4& matrix, const glm::vec3 move) {
        return glm::translate(glm::mat4(1.0f), move) * matrix;
    }

    glm::mat4 TransformationUtil::rotate(const glm::mat4& matrix, const float angleDegree, const glm::vec3 rotate) {
        return glm::rotate(matrix, glm::radians(angleDegree), rotate);
    }

    glm::mat4 TransformationUtil::rotateWorld(const glm::mat4& matrix, const float angleDegree, const glm::vec3 rotate) {
        return glm::rotate(glm::mat4(1.0f), glm::radians(angleDegree), rotate) * matrix;
    }

    glm::mat4 TransformationUtil::scaleWorld(const glm::mat4& matrix, const glm::vec3 scale) {
        return glm::scale(glm::mat4(1.0f), scale) * matrix;
    }

    glm::mat4 TransformationUtil::scale(const glm::mat4& matrix, const glm::vec3 scale) {
        return glm::scale(matrix, scale);
    }

    glm::mat4 TransformationUtil::setPosition(const glm::mat4& matrix, const glm::vec3 position) {
        glm::mat4 result = matrix;
        result[3] = glm::vec4(position, 1.0f);
        return result;
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

    void TransformationUtil::setWorldPosition(const Entity entity, const glm::vec3 position) {
        auto& [localModel, worldModel] = entity.getComponent<TransformationComponent>();
        worldModel[3] = glm::vec4(position, 1.0f);

        if (entity.hasAllComponents<PhysicsComponent>()) {
            Engine::getEngine()->getPhysicsManager().setPosition(entity.getComponent<PhysicsComponent>().bodyID, position);
        } else {
            localModel[3] = glm::vec4(position, 1.0f);
        }

        updateChildrenWorld(entity);
    }

    void TransformationUtil::updateChildrenWorld(const Entity entity) {
        if (!entity.hasAllComponents<ChildrenComponent>()) {
            return;
        }
        const glm::mat4 parentWorld = entity.getComponent<TransformationComponent>().worldModel;

        for (const auto child : entity.getComponent<ChildrenComponent>().children) {
            auto childEntity = Entity(entity.getScene(), child);
            if (!childEntity.hasAllComponents<TransformationComponent>()) {
                continue;
            }
            auto& [localModel, worldModel] = childEntity.getComponent<TransformationComponent>();
            worldModel = parentWorld * localModel;
            updateChildrenWorld(childEntity);
        }
    }
} // Kita
