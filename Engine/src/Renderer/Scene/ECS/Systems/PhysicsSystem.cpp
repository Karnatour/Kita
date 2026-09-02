#include "../../../../kitapch.h"
#include "PhysicsSystem.h"

#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../Components/Components.h"

namespace Kita {
    int PhysicsSystem::getOrder() {
        return Order::PHYSICS;
    }

    void PhysicsSystem::update(Scene& scene) {
        const auto& physicsManager = Engine::getEngine()->getPhysicsManager();

        for (auto [entityID, physics, transformation] : scene.view<PhysicsComponent, TransformationComponent>().each()) {
            const glm::mat4 physicsModelMatrix = physicsManager.getModelMatrix(physics.bodyID);

            transformation.worldModel = physicsModelMatrix;
            Entity entity = Entity(&scene, entityID);
            if (entity.hasAllComponents<ChildrenComponent>()) {
                for (const auto child : entity.getComponent<ChildrenComponent>().children) {
                    auto childEntity = Entity(&scene, child);
                    if (childEntity.hasAllComponents<TransformationComponent>()) {
                        syncTransformation(childEntity, physicsModelMatrix);
                    }
                }
            }
        }
    }

    void PhysicsSystem::render(Scene& scene) {
    }

    void PhysicsSystem::syncTransformation(Entity entity, const glm::mat4& parentModelMatrix) {
        auto& [localModel, worldModel] = entity.getComponent<TransformationComponent>();
        worldModel = parentModelMatrix * localModel;

        if (!entity.hasAllComponents<TransformationComponent>()) {
            return;
        }

        for (const auto child : entity.getComponent<ChildrenComponent>().children) {
            auto childEntity = Entity(entity.getScene(), child);
            if (childEntity.hasAllComponents<TransformationComponent>()) {
                syncTransformation(childEntity, worldModel);
            }
        }
    }
} // Kita
