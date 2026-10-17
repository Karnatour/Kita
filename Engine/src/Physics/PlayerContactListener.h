#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>

namespace Kita {
    class PlayerContactListener : public JPH::CharacterContactListener {
    public:
        explicit PlayerContactListener(const bool& allowSliding);

        void OnContactSolve(const JPH::CharacterVirtual* inCharacter, const JPH::BodyID& inBodyID2,
                            const JPH::SubShapeID& inSubShapeID2, JPH::RVec3Arg inContactPosition,
                            JPH::Vec3Arg inContactNormal, JPH::Vec3Arg inContactVelocity,
                            const JPH::PhysicsMaterial* inContactMaterial, JPH::Vec3Arg inCharacterVelocity,
                            JPH::Vec3& ioNewCharacterVelocity) override;

    private:
        const bool& m_allowSliding;
    };
} // Kita
