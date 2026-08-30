#include "../kitapch.h"
#include "ObjectLayerVsBroadPhaseLayerFilter.h"

#include "BroadPhaseLayer.h"
#include "PhysicsManager.h"

namespace Kita
{
    bool ObjectLayerVsBroadPhaseLayerFilter::ShouldCollide(const JPH::ObjectLayer objectLayer, const JPH::BroadPhaseLayer broadPhaseLayer) const
    {
        switch (objectLayer)
        {
        case PhysicsLayers::Layers::STATIC:
            return broadPhaseLayer == BroadPhaseLayer::MOVING_BP_LAYER;
        case PhysicsLayers::Layers::MOVING:
            return true;
        default:
            KITA_ENGINE_ASSERT(false, "Unknown PhysicsManager::Layers type");
        }
    }
} // Kita
