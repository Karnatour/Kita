#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

namespace Kita
{
    class BodyActivationListener : public JPH::BodyActivationListener
    {
    public:
        void OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;
        void OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;
    };
} // Kita
