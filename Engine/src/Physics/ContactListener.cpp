#include "../kitapch.h"
#include "ContactListener.h"

namespace Kita {
    JPH::ValidateResult ContactListener::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2,
                                                           JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) {
        KITA_ENGINE_DEBUG("[PhysicsManager] Contact has been validated");
        return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
    }

    void ContactListener::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2,
                                         const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) {
        KITA_ENGINE_DEBUG("[PhysicsManager] Contact has been added");
    }

    void ContactListener::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2,
                                             const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) {
        KITA_ENGINE_DEBUG("[PhysicsManager] Contact has been persisted");
    }

    void ContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) {
        KITA_ENGINE_DEBUG("[PhysicsManager] Contact has been removed");
    }
} // Kita
