#pragma once
#include <glm/vec3.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>

namespace Kita {
    struct PlayerCharacterProperties {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        //Physical Properties
        float mass = 70.0f; // Kg
        float maxStrength = 100.0f; // N

        //Movement
        bool allowControlWhileInAir = true;
        float speed = 6.0f;
        float jumpSpeed = 4.0f;
        bool isCurrentlyJumping = false;

        //Size
        float characterRadiusStanding = 0.3f;
        float characterHeightStanding = 1.35f;
        float characterRadiusCrouching = 0.3f;
        float characterHeightCrouching = 0.8f;
        float innerShapeFraction = 0.9f;

        //JPH Internal shouldn't be used by user
        JPH::Vec3 desiredVelocity = JPH::Vec3::sZero();
        JPH::Ref<JPH::CharacterVirtual> character;
        JPH::Ref<JPH::Shape> currentShape;
    };
} // Kita
