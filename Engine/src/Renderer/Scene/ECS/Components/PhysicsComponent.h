#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

namespace Kita {
    struct PhysicsComponent {
        JPH::BodyID bodyID;
    };
}
