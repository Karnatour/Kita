#include "PhysicsSystem.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>

#include "../../Scene.h"
#include "../../../../kitapch.h"
#include "../../../../Core/Engine.h"
#include "../../../../Events/EventManager.h"
#include "../../../../Events/PhysicsManagerEvents.h"
#include "../../../../Input/Input.h"
#include "../../../Util/PhysicsUtil.h"
#include "../Components/Components.h"

namespace Kita {
    PhysicsSystem::PhysicsSystem(Scene& scene) :
        m_scene(scene) {
    }

    int PhysicsSystem::getOrder() {
        return Order::PHYSICS;
    }

    void PhysicsSystem::update(Scene& scene) {
        if (Engine::getEngine()->isFirstFrame()) {
            initPlayerCharacterComponents();
            EventManager::listenToEvent<PreUpdateEvent>([this](const PreUpdateEvent& event) {
                preUpdatePlayerCharacter(event);
            });
        }
        handlePlayerCharacterInput();

        updateModelMatrices();
    }

    void PhysicsSystem::render(Scene& scene) {
    }

    void PhysicsSystem::updateModelMatrices() {
        const auto& physicsManager = Engine::getEngine()->getPhysicsManager();

        for (auto [entityID, physics, transformation] : m_scene.view<PhysicsComponent, TransformationComponent>().each()) {
            const glm::mat4 physicsModelMatrix = physicsManager.getModelMatrix(physics.bodyID);

            transformation.worldModel = physicsModelMatrix;
            Entity entity = Entity(&m_scene, entityID);
            if (entity.hasAllComponents<ChildrenComponent>()) {
                for (const auto child : entity.getComponent<ChildrenComponent>().children) {
                    auto childEntity = Entity(&m_scene, child);
                    if (childEntity.hasAllComponents<TransformationComponent>()) {
                        syncTransformation(childEntity, physicsModelMatrix);
                    }
                }
            }
        }
    }

    void PhysicsSystem::initPlayerCharacterComponents() {
        auto& physicsManager = Engine::getEngine()->getPhysicsManager();

        for (auto [entityID, playerCharacter] : m_scene.view<PlayerCharacterComponent>().each()) {
            PlayerCharacterProperties& p = playerCharacter.properties;

            const auto standingPos = JPH::Vec3(0.0f, 0.5f * p.heightStanding + p.radiusStanding, 0.0f);
            const auto crouchingPos = JPH::Vec3(0.0f, 0.5f * p.heightCrouching + p.radiusCrouching, 0.0f);

            p.standingShape = JPH::RotatedTranslatedShapeSettings(standingPos, JPH::Quat::sIdentity(), new JPH::CapsuleShape(0.5f * p.heightStanding, p.radiusStanding)).Create().Get();
            p.crouchingShape = JPH::RotatedTranslatedShapeSettings(crouchingPos, JPH::Quat::sIdentity(), new JPH::CapsuleShape(0.5f * p.heightCrouching, p.radiusCrouching)).Create().Get();
            p.innerStandingShape = JPH::RotatedTranslatedShapeSettings(standingPos, JPH::Quat::sIdentity(), new JPH::CapsuleShape(0.5f * p.innerShapeFraction * p.heightStanding, p.innerShapeFraction * p.radiusStanding)).Create().Get();
            p.innerCrouchingShape = JPH::RotatedTranslatedShapeSettings(crouchingPos, JPH::Quat::sIdentity(), new JPH::CapsuleShape(0.5f * p.innerShapeFraction * p.heightCrouching, p.innerShapeFraction * p.radiusCrouching)).Create().Get();

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

    void PhysicsSystem::handlePlayerCharacterInput() {
        for (auto [entityID, playerCharacter] : m_scene.view<PlayerCharacterComponent, ActiveCamera>().each()) {
            PlayerCharacterProperties& p = playerCharacter.properties;

            p.isCurrentlyJumping = false;
            p.isSwitchingStance = false;
            p.controlInput = JPH::Vec3::sZero();
            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_A)) {
                p.controlInput.SetZ(-1);
            }
            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_D)) {
                p.controlInput.SetZ(1);
            }
            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_W)) {
                p.controlInput.SetX(1);
            }
            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_S)) {
                p.controlInput.SetX(-1);
            }

            p.isCurrentlyJumping = Input::isKeyPressed(InputKeys::KeyboardKey::KEY_SPACE);
            p.isSwitchingStance = Input::isKeyPressed(InputKeys::KeyboardKey::KEY_LEFT_CONTROL);
        }
    }

    //https://github.com/jrouwe/JoltPhysics/blob/master/Samples/Tests/Character/CharacterVirtualTest.cpp
    void PhysicsSystem::preUpdatePlayerCharacter(const PreUpdateEvent& event) {
        for (auto [entityID, playerCharacter, camera] : m_scene.view<PlayerCharacterComponent, CameraComponent>().each()) {
            PlayerCharacterProperties& p = playerCharacter.properties;
            JPH::CharacterVirtual::ExtendedUpdateSettings updateSettings;

            updateSettings.mStickToFloorStepDown = -p.character->GetUp() * updateSettings.mStickToFloorStepDown.Length();
            updateSettings.mWalkStairsStepUp = p.character->GetUp() * updateSettings.mWalkStairsStepUp.Length();

            auto& physicsSystem = Engine::getEngine()->getPhysicsManager().getPhysicsSystem();
            auto deltaTime = static_cast<float>(event.getFixedDeltaTime());
            p.character->ExtendedUpdate(
                deltaTime,
                -p.character->GetUp() * physicsSystem.GetGravity().Length(),
                updateSettings,
                physicsSystem.GetDefaultBroadPhaseLayerFilter(PhysicsLayers::Layers::MOVING),
                physicsSystem.GetDefaultLayerFilter(PhysicsLayers::Layers::MOVING),
                {},
                {},
                Engine::getEngine()->getPhysicsManager().getTempAllocator());

            p.position = PhysicsUtil::JPHToGLMVec3(p.character->GetPosition());
            camera.properties.position = p.position;

            if (p.controlInput != JPH::Vec3::sZero()) {
                p.controlInput = p.controlInput.Normalized();
            }

            JPH::Vec3 front = PhysicsUtil::GLMToJPHVec3(camera.properties.front);
            front.SetY(0.0f);
            front = front.NormalizedOr(JPH::Vec3::sAxisX());
            JPH::Quat rotation = JPH::Quat::sFromTo(JPH::Vec3::sAxisX(), front);
            p.controlInput = rotation * p.controlInput;

            if (p.allowControlWhileInAir) {
                p.desiredVelocity = 0.25f * p.controlInput * p.speed + 0.75f * p.desiredVelocity;

                p.allowSliding = !p.controlInput.IsNearZero();
            }

            JPH::Quat characterUpRotation = JPH::Quat::sEulerAngles(JPH::Vec3(0.0f, 0.0f, 0.0f));
            p.character->SetUp(characterUpRotation.RotateAxisY());
            p.character->SetRotation(characterUpRotation);

            JPH::Vec3 currentVerticalVelocity = p.character->GetLinearVelocity().Dot(p.character->GetUp()) * p.character->GetUp();
            JPH::Vec3 groundVelocity = p.character->GetGroundVelocity();
            JPH::Vec3 newVelocity = JPH::Vec3::sZero();
            bool movingTowardsGround = (currentVerticalVelocity.GetY() - groundVelocity.GetY()) < 0.1f;
            if (p.character->GetGroundState() == JPH::CharacterBase::EGroundState::OnGround && movingTowardsGround) {
                newVelocity = groundVelocity;

                if (p.isCurrentlyJumping && movingTowardsGround) {
                    newVelocity = newVelocity + p.jumpSpeed * p.character->GetUp();
                }
            } else {
                newVelocity = currentVerticalVelocity;
            }

            //Gravity
            newVelocity = newVelocity + (characterUpRotation * physicsSystem.GetGravity()) * deltaTime;

            if (p.allowControlWhileInAir) {
                newVelocity = newVelocity + characterUpRotation * p.desiredVelocity;
            } else {
                JPH::Vec3 currentHorizontalVelocity = p.character->GetLinearVelocity() - currentVerticalVelocity;
                newVelocity = newVelocity + currentHorizontalVelocity;
            }

            p.character->SetLinearVelocity(newVelocity);

            if (p.isSwitchingStance) {
                bool isStanding = p.character->GetShape() == p.standingShape;
                const JPH::Shape* shape = isStanding ? p.crouchingShape : p.standingShape;
                if (p.character->SetShape(
                    shape,
                    1.5f * physicsSystem.GetPhysicsSettings().mPenetrationSlop,
                    physicsSystem.GetDefaultBroadPhaseLayerFilter(PhysicsLayers::Layers::MOVING),
                    physicsSystem.GetDefaultLayerFilter(PhysicsLayers::Layers::MOVING),
                    {},
                    {},
                    Engine::getEngine()->getPhysicsManager().getTempAllocator())) {
                    const JPH::Shape* innerShape = isStanding ? p.innerCrouchingShape : p.innerStandingShape;
                    p.character->SetInnerBodyShape(innerShape);
                }
            }
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
