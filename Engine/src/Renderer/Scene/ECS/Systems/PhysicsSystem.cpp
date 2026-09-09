#include "../../../../kitapch.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

#include "PhysicsSystem.h"

#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../../../Util/PhysicsUtil.h"
#include "../Components/Components.h"

namespace Kita {
    int PhysicsSystem::getOrder() {
        return Order::PHYSICS;
    }

    void PhysicsSystem::update(Scene& scene) {
        if (Engine::getEngine()->isFirstFrame()) {
            initPlayerCharacterComponents(scene);
        }
        updateModelMatrices(scene);
    }

    void PhysicsSystem::render(Scene& scene) {
    }

    void PhysicsSystem::updateModelMatrices(Scene& scene) {
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

    void PhysicsSystem::initPlayerCharacterComponents(Scene& scene) {
        auto& physicsManager = Engine::getEngine()->getPhysicsManager();

        for (auto [entityID, playerCharacter] : scene.view<PlayerCharacterComponent>().each()) {
            PlayerCharacterProperties& p = playerCharacter.properties;

            const auto standingPos = JPH::Vec3(0.0f, 0.5f * p.heightStanding + p.radiusStanding, 0.0f);
            const auto crouchingPos = JPH::Vec3(0.0f, 0.5f * p.heightCrouching + p.radiusCrouching, 0.0f);

            p.standingShape = JPH::RotatedTranslatedShapeSettings(standingPos, JPH::Quat::sIdentity(), new JPH::CapsuleShape(0.5 * p.heightStanding, p.radiusStanding)).Create().Get();
            p.crouchingShape = JPH::RotatedTranslatedShapeSettings(crouchingPos, JPH::Quat::sIdentity(), new JPH::CapsuleShape(0.5 * p.heightCrouching, p.radiusCrouching)).Create().Get();
            p.innerStandingShape = JPH::RotatedTranslatedShapeSettings(standingPos, JPH::Quat::sIdentity(), new JPH::CapsuleShape(0.5 * p.innerShapeFraction * p.heightStanding, p.innerShapeFraction * p.radiusStanding)).Create().Get();
            p.innerCrouchingShape = JPH::RotatedTranslatedShapeSettings(crouchingPos, JPH::Quat::sIdentity(), new JPH::CapsuleShape(0.5 * p.innerShapeFraction * p.heightCrouching, p.innerShapeFraction * p.radiusCrouching)).Create().Get();

            JPH::Ref settings = new JPH::CharacterVirtualSettings();
            settings->mShape = p.standingShape;
            settings->mInnerBodyShape = p.innerStandingShape;
            settings->mInnerBodyLayer = PhysicsLayers::MOVING;
            settings->mEnhancedInternalEdgeRemoval = true;
            settings->mMass = p.mass;
            settings->mMaxStrength = p.maxStrength;
            settings->mSupportingVolume = JPH::Plane(JPH::Vec3::sAxisY(), -p.radiusStanding);

            p.character = new JPH::CharacterVirtual(settings, PhysicsUtil::GLMToJPHRVec3(p.position), JPH::Quat::sIdentity(), 0, &physicsManager.getPhysicsSystem());
        }
    }

    void PhysicsSystem::syncTransformation(const Entity entity, const glm::mat4& parentModelMatrix) {
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
