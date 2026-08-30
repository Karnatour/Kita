#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>

namespace Kita
{
    class ObjectLayerPairFilter : public JPH::ObjectLayerPairFilter
    {
    public:
        // Determines if two object layers can collide
        bool ShouldCollide(JPH::ObjectLayer firstLayerType, JPH::ObjectLayer secondLayerType) const override;
    };
} // Kita