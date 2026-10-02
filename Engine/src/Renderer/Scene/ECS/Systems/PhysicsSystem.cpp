#include "PhysicsSystem.h"

#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Vehicle/WheeledVehicleController.h>

#include "CameraSystem.h"
#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../../../../Events/EventManager.h"
#include "../../../../Events/PhysicsManagerEvents.h"
#include "../../../../Input/Input.h"
#include "../../../Util/PhysicsUtil.h"
#include "../../../Util/TransformationUtil.h"
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
            initVehicleComponents();
            EventManager::listenToEvent<PreUpdateEvent>([this](const PreUpdateEvent& event) {
                preUpdatePlayerCharacter(event);
                preUpdateVehicle(event);
            });
        }
        handlePlayerCharacterInput();
        handleVehicleInput();

        updateModelMatrices();
    }

    void PhysicsSystem::render(Scene& scene) {
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
            settings->mEnhancedInternalEdgeRemoval = false;
            settings->mMass = p.mass;
            settings->mMaxStrength = p.maxStrength;
            settings->mSupportingVolume = JPH::Plane(JPH::Vec3::sAxisY(), -p.radiusStanding);

            p.character = new JPH::CharacterVirtual(settings, PhysicsUtil::GLMToJPHRVec3(p.position), JPH::Quat::sIdentity(), 0, &physicsManager.getPhysicsSystem());
        }
    }

    // https://github.com/jrouwe/JoltPhysics/blob/master/Samples/Tests/Character/CharacterVirtualTest.cpp
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

            bool isStanding = p.character->GetShape() == p.standingShape;
            float currentHeight = isStanding ? p.heightStanding : p.heightCrouching;
            float currentRadius = isStanding ? p.radiusStanding : p.radiusCrouching;
            float eyeY = currentHeight + 2.0f * currentRadius - p.eyeOffset;

            p.position = PhysicsUtil::JPHToGLMVec3(p.character->GetPosition() + JPH::Vec3(0.0f, eyeY, 0.0f));
            camera.properties.position = p.position;

            if (p.controlInput != JPH::Vec3::sZero()) {
                p.controlInput = p.controlInput.Normalized();
            }

            JPH::Vec3 front = PhysicsUtil::GLMToJPHVec3(camera.properties.front);
            front.SetY(0.0f);
            front = front.NormalizedOr(JPH::Vec3::sAxisX());
            JPH::Quat rotation = JPH::Quat::sFromTo(JPH::Vec3::sAxisX(), front);
            p.controlInput = rotation * p.controlInput;

            float speed = p.isCurrentlySprinting ? p.speed * 2.0f : p.speed;
            if (p.allowControlWhileInAir) {
                p.desiredVelocity = 0.25f * p.controlInput * speed + 0.75f * p.desiredVelocity;

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
                p.isSwitchingStance = false;
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

    void PhysicsSystem::handlePlayerCharacterInput() {
        for (auto [entityID, playerCharacter] : m_scene.view<PlayerCharacterComponent, ActiveCamera>().each()) {
            PlayerCharacterProperties& p = playerCharacter.properties;
            glm::vec3 direction(0.0f);

            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_W)) {
                direction += glm::vec3(1.0f, 0.0f, 0.0f);
            }
            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_A)) {
                direction += glm::vec3(0.0f, 0.0f, -1.0f);
            }
            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_S)) {
                direction += glm::vec3(-1.0f, 0.0f, 0.0f);
            }
            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_D)) {
                direction += glm::vec3(0.0f, 0.0f, 1.0f);
            }
            const glm::vec3 normalizedDirection = (glm::length2(direction) > 0.0f) ? glm::normalize(direction) : glm::vec3(0.0f);
            p.controlInput = PhysicsUtil::GLMToJPHVec3(normalizedDirection);

            p.isCurrentlyJumping = Input::isKeyPressed(InputKeys::KeyboardKey::KEY_SPACE);
            bool switchStanceKeyDown = Input::isKeyPressed(InputKeys::KeyboardKey::KEY_LEFT_CONTROL);
            if (switchStanceKeyDown && !p.wasSwitchingStance) {
                p.isSwitchingStance = true;
            }
            p.wasSwitchingStance = switchStanceKeyDown;

            p.isCurrentlySprinting = Input::isKeyPressed(InputKeys::KeyboardKey::KEY_LEFT_SHIFT);
        }
    }

    void PhysicsSystem::initVehicleComponents() {
        auto& physicsManager = Engine::getEngine()->getPhysicsManager();

        for (auto [entityID, vehicleComponent, physicsCmp] : m_scene.view<VehicleComponent, PhysicsComponent>().each()) {
            VehicleProperties& p = vehicleComponent.properties;

            const glm::vec3 wheelCenter = getWheelCenter(p);
            const JPH::Vec3 boxCenter(0.0f, wheelCenter.y + p.halfVehicleHeight, 0.0f);
            JPH::RefConst carShape = JPH::OffsetCenterOfMassShapeSettings(
                JPH::Vec3(0, -p.halfVehicleHeight, 0),
                new JPH::RotatedTranslatedShape(boxCenter, JPH::Quat::sIdentity(), new JPH::BoxShape(JPH::Vec3(p.halfVehicleWidth, p.halfVehicleHeight, p.halfVehicleLength))
                )).Create().Get();
            auto carBodySettings = JPH::BodyCreationSettings(carShape, PhysicsUtil::GLMToJPHVec3(p.position), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, PhysicsLayers::Layers::MOVING);
            carBodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
            carBodySettings.mMassPropertiesOverride.mMass = p.vehicleWeight;

            physicsCmp.bodyID = physicsManager.createBody(carBodySettings, JPH::EActivation::Activate);

            JPH::VehicleConstraintSettings vehicle;
            vehicle.mMaxPitchRollAngle = p.maxRollAngle;

            std::array<JPH::Ref<JPH::WheelSettingsWV>, 4> vehicleWheels = getVehicleWheels(p);
            for (auto& wheel : vehicleWheels) {
                vehicle.mWheels.emplace_back(std::move(wheel));
            }

            JPH::Ref controller = new JPH::WheeledVehicleControllerSettings;
            vehicle.mController = controller;

            controller->mDifferentials.resize(p.fourWheelDrive ? 2 : 1);
            controller->mDifferentials[0].mLeftWheel = 0;
            controller->mDifferentials[0].mRightWheel = 1;
            if (p.fourWheelDrive) {
                controller->mDifferentials[1].mLeftWheel = 2;
                controller->mDifferentials[1].mRightWheel = 3;

                controller->mDifferentials[0].mEngineTorqueRatio = controller->mDifferentials[1].mEngineTorqueRatio = 0.5f;
            }

            if (p.antiRollbar) {
                vehicle.mAntiRollBars.resize(2);
                vehicle.mAntiRollBars[0].mLeftWheel = 0;
                vehicle.mAntiRollBars[0].mRightWheel = 1;
                vehicle.mAntiRollBars[1].mLeftWheel = 2;
                vehicle.mAntiRollBars[1].mRightWheel = 3;
            }

            p.vehicleConstraint = new JPH::VehicleConstraint(*physicsManager.getPhysicsSystem().GetBodyLockInterfaceNoLock().TryGetBody(physicsCmp.bodyID), vehicle);

            p.vehicleConstraint->SetVehicleCollisionTester(new JPH::VehicleCollisionTesterRay(PhysicsLayers::Layers::MOVING));

            physicsManager.getPhysicsSystem().AddConstraint(p.vehicleConstraint);
            physicsManager.getPhysicsSystem().AddStepListener(p.vehicleConstraint);
        }
    }

    JPH::Ref<JPH::WheelSettingsWV> PhysicsSystem::makeWheel(entt::entity wheelEnttID, const bool isFront, const bool isLeft, const VehicleProperties& p) const {
        auto [min, max] = getWheelMinMaxPos(wheelEnttID);

        glm::vec3 size = max - min;

        float width, diameter;
        if (size.x <= size.y && size.x <= size.z) {
            width = size.x;
            diameter = glm::max(size.y, size.z);
        } else if (size.y <= size.z) {
            width = size.y;
            diameter = glm::max(size.x, size.z);
        } else {
            width = size.z;
            diameter = glm::max(size.x, size.y);
        }

        const float suspensionMinLen = isFront ? p.frontSuspensionMinLength : p.rearSuspensionMinLength;
        const float suspensionMaxLen = isFront ? p.frontSuspensionMaxLength : p.rearSuspensionMaxLength;
        const float suspensionMidLen = 0.5f * (suspensionMinLen + suspensionMaxLen);

        const float sidewaysAngle = isFront ? p.frontSuspensionSidewaysAngle : p.rearSuspensionSidewaysAngle;
        const float forwardAngle = isFront ? p.frontSuspensionForwardAngle : p.rearSuspensionForwardAngle;
        const float kingPinAngle = isFront ? p.frontKingPinAngle : p.rearKingPinAngle;
        const float casterAngle = isFront ? p.frontCasterAngle : p.rearCasterAngle;
        const float camberAngle = isFront ? p.frontCamber : p.rearCamber;
        const float toeAngle = isFront ? p.frontToe : p.rearToe;

        auto suspensionDirection = JPH::Vec3(JPH::Tan(sidewaysAngle), -1, JPH::Tan(forwardAngle)).Normalized();
        auto steeringAxis = JPH::Vec3(-JPH::Tan(kingPinAngle), 1, -JPH::Tan(casterAngle)).Normalized();
        auto wheelUp = JPH::Vec3(JPH::Sin(camberAngle), JPH::Cos(camberAngle), 0);
        auto wheelForward = JPH::Vec3(-JPH::Sin(toeAngle), 0, JPH::Cos(toeAngle));

        if (!isLeft) {
            JPH::Vec3 flipX(-1, 1, 1);
            suspensionDirection = flipX * suspensionDirection;
            steeringAxis = flipX * steeringAxis;
            wheelUp = flipX * wheelUp;
            wheelForward = flipX * wheelForward;
        }

        glm::vec3 center = (min + max) * 0.5f;
        glm::vec3 attach = center + glm::vec3(0.0f, suspensionMidLen, 0.0f);
        JPH::Vec3 position = PhysicsUtil::GLMToJPHVec3(attach);

        JPH::Ref wheel = new JPH::WheelSettingsWV;
        wheel->mPosition = position;
        wheel->mRadius = 0.5f * diameter;
        wheel->mWidth = width;
        wheel->mSuspensionDirection = suspensionDirection;
        wheel->mSteeringAxis = steeringAxis;
        wheel->mWheelUp = wheelUp;
        wheel->mWheelForward = wheelForward;
        wheel->mSuspensionMinLength = isFront ? p.frontSuspensionMinLength : p.rearSuspensionMinLength;
        wheel->mSuspensionMaxLength = isFront ? p.frontSuspensionMaxLength : p.rearSuspensionMaxLength;
        wheel->mSuspensionSpring.mFrequency = isFront ? p.frontSuspensionFrequency : p.rearSuspensionFrequency;
        wheel->mSuspensionSpring.mDamping = isFront ? p.frontSuspensionDamping : p.rearSuspensionDamping;
        wheel->mMaxSteerAngle = isFront ? p.maxSteeringAngle : 0.0f;
        if (isFront) {
            wheel->mMaxHandBrakeTorque = 0.0f;
        }
        return wheel;
    }

    glm::vec3 PhysicsSystem::getWheelCenter(const VehicleProperties& p) {
        auto c = [this](entt::entity e) {
            auto [min, max] = getWheelMinMaxPos(e);
            return (min + max) * 0.5f;
        };
        glm::vec3 center = 0.25f * (c(p.frontLeftWheelEnttID) + c(p.frontRightWheelEnttID) + c(p.rearLeftWheelEnttID) + c(p.rearRightWheelEnttID));
        center.x = 0.0f;
        return center;
    }

    std::array<JPH::Ref<JPH::WheelSettingsWV>, 4> PhysicsSystem::getVehicleWheels(VehicleProperties& p) {
        auto getCenter = [](const std::pair<glm::vec3, glm::vec3>& minMaxPos) {
            return (minMaxPos.first + minMaxPos.second) * 0.5f;
        };

        return {
            makeWheel(p.frontLeftWheelEnttID, true, true, p),
            makeWheel(p.frontRightWheelEnttID, true, false, p),
            makeWheel(p.rearLeftWheelEnttID, false, true, p),
            makeWheel(p.rearRightWheelEnttID, false, false, p),
        };
    }

    std::pair<glm::vec3, glm::vec3> PhysicsSystem::getWheelMinMaxPos(const entt::entity wheelEnttID) const {
        const Entity wheelEntity(&m_scene, wheelEnttID);
        const glm::mat4 localModel = wheelEntity.getComponent<TransformationComponent>().localModel;

        glm::vec3 min(FLT_MAX);
        glm::vec3 max(-FLT_MAX);

        auto meshID = Entity(wheelEntity.getScene(), wheelEntity.getComponent<ChildrenComponent>().children.front()).getComponent<MeshComponent>().meshID;
        for (auto& element : Engine::getEngine()->getAssetManager().getAsset<Mesh>(meshID).getVertexBuffer().getVertices()) {
            auto localPos = glm::vec3(localModel * glm::vec4(element.position, 1.0f));
            min = glm::min(min, localPos);
            max = glm::max(max, localPos);
        }

        return std::make_pair(min, max);
    }

    // https://github.com/jrouwe/JoltPhysics/blob/master/Samples/Tests/Vehicle/VehicleConstraintTest.cpp
    void PhysicsSystem::preUpdateVehicle(const PreUpdateEvent& event) {
        for (auto [entityID,physicsCmp,vehicle,camera, transformation] : m_scene.view<PhysicsComponent, VehicleComponent, CameraComponent, TransformationComponent>().each()) {
            VehicleProperties& p = vehicle.properties;

            // Park the vehicle if player is not inside
            if (vehicle.properties.parked) {
                p.forward = 0.0f;
                p.right = 0.0f;
                p.brake = 1.0f;
                p.handBrake = 1.0f;
            }

            if (p.forward != 0.0f || p.right != 0.0f || p.brake != 0.0f || p.handBrake != 0.0f) {
                Engine::getEngine()->getPhysicsManager().getPhysicsSystem().GetBodyInterface().ActivateBody(physicsCmp.bodyID);
            }

            JPH::Vec3 forward = p.vehicleConstraint->GetVehicleBody()->GetRotation().RotateAxisZ();
            forward.SetY(0.0f);
            const float length = forward.Length();
            if (length != 0.0f) {
                forward = forward / length;
            } else {
                forward = JPH::Vec3::sAxisZ();
            }

            const glm::vec3 carPos = PhysicsUtil::JPHToGLMVec3(p.vehicleConstraint->GetVehicleBody()->GetPosition());
            camera.properties.position = carPos - PhysicsUtil::JPHToGLMVec3(forward * 6.25f) + glm::vec3(0.0f, 2.5f, 0.0f);

            const glm::vec3 front = glm::normalize(carPos - camera.properties.position);
            camera.properties.pitch = glm::degrees(std::asin(front.y));
            camera.properties.yaw = glm::degrees(std::atan2(front.z, front.x));
            CameraUtil::updateOrientationVectors(camera.properties);

            auto controller = static_cast<JPH::WheeledVehicleController*>(p.vehicleConstraint->GetController());
            controller->GetEngine().mMaxTorque = p.maxEngineTorque;
            controller->GetTransmission().mClutchStrength = p.clutchStrength;

            const float limitedSlipDifferentialVal = p.limitedSlipDifferentials ? 1.4f : FLT_MAX;
            controller->SetDifferentialLimitedSlipRatio(limitedSlipDifferentialVal);
            for (auto& differential : controller->GetDifferentials()) {
                differential.mLimitedSlipRatio = limitedSlipDifferentialVal;
            }

            controller->SetDriverInput(p.forward, p.right, p.brake, p.handBrake);

            transformation.worldModel = PhysicsUtil::JPHToGLMMat4(p.vehicleConstraint->GetVehicleBody()->GetWorldTransform());

            Entity(&m_scene, p.frontLeftWheelEnttID).getComponent<TransformationComponent>().localModel = PhysicsUtil::JPHToGLMMat4(p.vehicleConstraint->GetWheelLocalTransform(0, -JPH::Vec3::sAxisX(), JPH::Vec3::sAxisY()));
            Entity(&m_scene, p.frontRightWheelEnttID).getComponent<TransformationComponent>().localModel = PhysicsUtil::JPHToGLMMat4(p.vehicleConstraint->GetWheelLocalTransform(1, JPH::Vec3::sAxisX(), JPH::Vec3::sAxisY()));
            Entity(&m_scene, p.rearLeftWheelEnttID).getComponent<TransformationComponent>().localModel = PhysicsUtil::JPHToGLMMat4(p.vehicleConstraint->GetWheelLocalTransform(2, -JPH::Vec3::sAxisX(), JPH::Vec3::sAxisY()));
            Entity(&m_scene, p.rearRightWheelEnttID).getComponent<TransformationComponent>().localModel = PhysicsUtil::JPHToGLMMat4(p.vehicleConstraint->GetWheelLocalTransform(3, JPH::Vec3::sAxisX(), JPH::Vec3::sAxisY()));
        }
    }

    void PhysicsSystem::handleVehicleInput() {
        for (auto [entityID, vehicle] : m_scene.view<VehicleComponent, ActiveCamera>().each()) {
            VehicleProperties& p = vehicle.properties;
            p.forward = 0.0f;
            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_W)) {
                p.forward = 1.0f;
            } else if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_S)) {
                p.forward = -1.0f;
            }

            p.brake = 0.0f;
            if (p.previousForward * p.forward < 0.0f) {
                float velocity = (p.vehicleConstraint->GetVehicleBody()->GetRotation().Conjugated() * p.vehicleConstraint->GetVehicleBody()->GetLinearVelocity()).GetZ();
                if ((p.forward > 0.0f && velocity < -0.1f) || (p.forward < 0.0f && velocity > 0.1f)) {
                    p.forward = 0.0f;
                    p.brake = 1.0f;
                } else {
                    p.previousForward = p.forward;
                }
            }

            p.handBrake = 0.0f;
            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_SPACE)) {
                p.forward = 0.0f;
                p.handBrake = 1.0f;
            }

            p.right = 0.0f;
            if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_A)) {
                p.right = -1.0f;
            } else if (Input::isKeyPressed(InputKeys::KeyboardKey::KEY_D)) {
                p.right = 1.0f;
            }
        }
    }

    void PhysicsSystem::updateModelMatrices() {
        const auto& physicsManager = Engine::getEngine()->getPhysicsManager();

        for (auto [entityID, physics, transformation] : m_scene.view<PhysicsComponent, TransformationComponent>().each()) {
            if (physics.bodyID.IsInvalid()) {
                continue;
            }

            const glm::mat4 physicsModelMatrix = physicsManager.getModelMatrix(physics.bodyID);
            const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), TransformationUtil::decompose(transformation.worldModel).scale);
            transformation.worldModel = physicsModelMatrix * scaleMatrix;

            Entity entity = Entity(&m_scene, entityID);
            if (entity.hasAllComponents<ChildrenComponent>()) {
                for (const auto child : entity.getComponent<ChildrenComponent>().children) {
                    auto childEntity = Entity(&m_scene, child);
                    if (childEntity.hasAllComponents<TransformationComponent>()) {
                        syncTransformation(childEntity, transformation.worldModel);
                    }
                }
            }
        }
    }

    void PhysicsSystem::syncTransformation(const Entity entity, const glm::mat4& parentModelMatrix) {
        auto& [localModel, worldModel] = entity.getComponent<TransformationComponent>();
        worldModel = parentModelMatrix * localModel;

        if (!entity.hasAllComponents<ChildrenComponent>()) {
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
