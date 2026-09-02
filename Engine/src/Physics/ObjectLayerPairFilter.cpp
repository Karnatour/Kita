#include "../kitapch.h"
#include "ObjectLayerPairFilter.h"

#include "PhysicsManager.h"

namespace Kita {
    bool ObjectLayerPairFilter::ShouldCollide(const JPH::ObjectLayer firstLayerType, const JPH::ObjectLayer secondLayerType) const {
        switch (firstLayerType) {
            case PhysicsLayers::Layers::STATIC:
                return secondLayerType == PhysicsLayers::Layers::MOVING;
            case PhysicsLayers::Layers::MOVING:
                return true;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown PhysicsManager::Layers type");
                return false;
        }
    }
} // Kita
