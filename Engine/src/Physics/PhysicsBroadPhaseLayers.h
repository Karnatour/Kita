#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace Kita {
    struct PhysicsBroadPhaseLayers {
        static constexpr auto STATIC_BP_LAYER = JPH::BroadPhaseLayer(0);
        static constexpr auto MOVING_BP_LAYER = JPH::BroadPhaseLayer(1);
        static constexpr JPH::uint NUM_BP_LAYERS = 2;
    };
} // Kita
