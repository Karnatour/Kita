#include "../kitapch.h"
#include "BroadPhaseLayer.h"

namespace Kita
{
    BroadPhaseLayer::BroadPhaseLayer()
    {
        m_objectLayerToBroadPhaseLayer[PhysicsLayers::Layers::STATIC] = STATIC_BP_LAYER;
        m_objectLayerToBroadPhaseLayer[PhysicsLayers::Layers::MOVING] = MOVING_BP_LAYER;
    }

    JPH::uint BroadPhaseLayer::GetNumBroadPhaseLayers() const
    {
        return NUM_LAYERS;
    }

    JPH::BroadPhaseLayer BroadPhaseLayer::GetBroadPhaseLayer(const JPH::ObjectLayer inLayer) const
    {
        return m_objectLayerToBroadPhaseLayer[inLayer];
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* BroadPhaseLayer::GetBroadPhaseLayerName(const JPH::BroadPhaseLayer inLayer) const
    {
        switch (static_cast<JPH::BroadPhaseLayer::Type>(inLayer))
        {
        case static_cast<JPH::BroadPhaseLayer::Type>(STATIC_BP_LAYER):
            return "STATIC";
        case static_cast<JPH::BroadPhaseLayer::Type>(MOVING_BP_LAYER):
            return "MOVING";
        default:
            KITA_ENGINE_ASSERT(false, "Unknown BroadPhase Layer");
            return "INVALID";
        }
    }
#endif
} // Kita
