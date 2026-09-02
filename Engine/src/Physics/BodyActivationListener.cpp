#include "../kitapch.h"
#include "BodyActivationListener.h"

namespace Kita {
    void BodyActivationListener::OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) {
        KITA_ENGINE_DEBUG("Body has been activated");
    }

    void BodyActivationListener::OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) {
        KITA_ENGINE_DEBUG("Body has been deactivated");
    }
} // Kita
