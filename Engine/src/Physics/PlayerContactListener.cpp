#include "PlayerContactListener.h"

namespace Kita {
    PlayerContactListener::PlayerContactListener(const bool& allowSliding) : m_allowSliding(allowSliding) {
    }

    void PlayerContactListener::OnContactSolve(const JPH::CharacterVirtual* inCharacter, const JPH::BodyID& inBodyID2, const JPH::SubShapeID& inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal,
                                               JPH::Vec3Arg inContactVelocity, const JPH::PhysicsMaterial* inContactMaterial, JPH::Vec3Arg inCharacterVelocity, JPH::Vec3& ioNewCharacterVelocity) {
        // Don't slide down static, not-too-steep surfaces when the player isn't actively moving
        if (!m_allowSliding && inContactVelocity.IsNearZero() && !inCharacter->IsSlopeTooSteep(inContactNormal)) {
            ioNewCharacterVelocity = JPH::Vec3::sZero();
        }
    }
} // Kita
