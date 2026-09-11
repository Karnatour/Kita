#pragma once
#include <glm/vec3.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>

namespace Kita {
    struct PlayerCharacterProperties {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        //Physical Properties
        float mass = 70.0f; // Kg
        float maxStrength = 100000.0f; // N

        //Movement
        bool allowControlWhileInAir = true;
        float speed = 6.0f;
        float jumpSpeed = 4.0f;
        bool isCurrentlySprinting = false;
        bool isCurrentlyJumping = false;
        bool isSwitchingStance = false;
        bool wasSwitchingStance = false;

        //Size
        float heightStanding = 1.35f;
        float radiusStanding = 0.3f;
        float heightCrouching = 0.8f;
        float radiusCrouching = 0.3f;
        float innerShapeFraction = 0.9f;

        //Camera
        float eyeOffset = 0.1f;

        //JPH Internal shouldn't be used by user
        JPH::Vec3 desiredVelocity = JPH::Vec3::sZero();
        JPH::Vec3 controlInput = JPH::Vec3::sZero();
        bool allowSliding = false;
        JPH::Ref<JPH::CharacterVirtual> character;
        JPH::Ref<JPH::Shape> currentShape;

        JPH::RefConst<JPH::Shape> standingShape;
        JPH::RefConst<JPH::Shape> crouchingShape;
        JPH::RefConst<JPH::Shape> innerCrouchingShape;
        JPH::RefConst<JPH::Shape> innerStandingShape;
    };
} // Kita
