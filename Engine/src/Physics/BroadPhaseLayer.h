#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include "PhysicsLayers.h"

namespace Kita {
    class BroadPhaseLayer : public JPH::BroadPhaseLayerInterface {
    public:
        BroadPhaseLayer();
        JPH::uint GetNumBroadPhaseLayers() const override;
        JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif

    private:
        JPH::BroadPhaseLayer m_objectLayerToBroadPhaseLayer[PhysicsLayers::Layers::COUNT] = {};
    };
} // Kita
