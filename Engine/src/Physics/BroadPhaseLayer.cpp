#include "../kitapch.h"
#include "BroadPhaseLayer.h"

#include "PhysicsBroadPhaseLayers.h"

namespace Kita {
    BroadPhaseLayer::BroadPhaseLayer() {
        m_objectLayerToBroadPhaseLayer[PhysicsLayers::Layers::STATIC] = PhysicsBroadPhaseLayers::STATIC_BP_LAYER;
        m_objectLayerToBroadPhaseLayer[PhysicsLayers::Layers::MOVING] = PhysicsBroadPhaseLayers::MOVING_BP_LAYER;
    }

    JPH::uint BroadPhaseLayer::GetNumBroadPhaseLayers() const {
        return PhysicsBroadPhaseLayers::NUM_BP_LAYERS;
    }

    JPH::BroadPhaseLayer BroadPhaseLayer::GetBroadPhaseLayer(const JPH::ObjectLayer inLayer) const {
        return m_objectLayerToBroadPhaseLayer[inLayer];
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* BroadPhaseLayer::GetBroadPhaseLayerName(const JPH::BroadPhaseLayer inLayer) const {
        switch (static_cast<JPH::BroadPhaseLayer::Type>(inLayer)) {
            case static_cast<JPH::BroadPhaseLayer::Type>(PhysicsBroadPhaseLayers::STATIC_BP_LAYER):
                return "STATIC";
            case static_cast<JPH::BroadPhaseLayer::Type>(PhysicsBroadPhaseLayers::MOVING_BP_LAYER):
                return "MOVING";
            default:
                KITA_ENGINE_ASSERT(false, "Unknown BroadPhase Layer");
                return "INVALID";
        }
    }
#endif
} // Kita
