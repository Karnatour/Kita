#pragma once
#include <glm/glm.hpp>
#include <entt/entity/entity.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Vehicle/VehicleConstraint.h>

namespace Kita {
    struct VehicleProperties {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        bool parked = false;

        float vehicleWeight = 1700.0f;

        float halfVehicleLength = 2.25f;
        float halfVehicleWidth = 0.92f;
        float halfVehicleHeight = 0.35f;

        float maxRollAngle = JPH::DegreesToRadians(60.0f);
        float maxSteeringAngle = JPH::DegreesToRadians(30.0f);
        bool fourWheelDrive = false;
        bool antiRollbar = true;
        bool limitedSlipDifferentials = true;
        float maxEngineTorque = 500.0f;
        float clutchStrength = 10.0f;
        float frontCasterAngle = 0.0f;
        float frontKingPinAngle = 0.0f;
        float frontCamber = 0.0f;
        float frontToe = 0.0f;
        float frontSuspensionForwardAngle = 0.0f;
        float frontSuspensionSidewaysAngle = 0.0f;
        float frontSuspensionMinLength = 0.15f;
        float frontSuspensionMaxLength = 0.30f;
        float frontSuspensionFrequency = 1.5f;
        float frontSuspensionDamping = 0.5f;
        float rearSuspensionForwardAngle = 0.0f;
        float rearSuspensionSidewaysAngle = 0.0f;
        float rearCasterAngle = 0.0f;
        float rearKingPinAngle = 0.0f;
        float rearCamber = 0.0f;
        float rearToe = 0.0f;
        float rearSuspensionMinLength = 0.15f;
        float rearSuspensionMaxLength = 0.30f;
        float rearSuspensionFrequency = 1.5f;
        float rearSuspensionDamping = 0.5f;

        //Wheels are stored also in this order in JPH
        entt::entity frontLeftWheelEnttID;
        entt::entity frontRightWheelEnttID;
        entt::entity rearLeftWheelEnttID;
        entt::entity rearRightWheelEnttID;

        //JPH Internal shouldn't be changed by user
        JPH::Ref<JPH::VehicleConstraint> vehicleConstraint;
        float forward = 0.0f;
        float previousForward = 1.0f;
        float right = 0.0f;
        float brake = 0.0f;
        float handBrake = 0.0f;
    };
} // Kita
