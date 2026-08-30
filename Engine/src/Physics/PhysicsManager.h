#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include "BodyActivationListener.h"
#include "BroadPhaseLayer.h"
#include "ContactListener.h"
#include "ObjectLayerPairFilter.h"
#include "ObjectLayerVsBroadPhaseLayerFilter.h"

namespace Kita
{
    class PhysicsManager
    {
    public:
        JPH::BodyID createBody(const JPH::BodyCreationSettings& creationSettings, bool activate);
        void removeBody(JPH::BodyID id);
        void destroyBody(JPH::BodyID id);
        bool isBodyActive(JPH::BodyID id) const;
        void activateBody(JPH::BodyID id);
        void deactivateBody(JPH::BodyID id);
    private:
        friend class Engine;

        void init();
        void update();
        void exit();
        static void TraceImpl(const char *inFMT, ...);
        static bool AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, JPH::uint inLine);
        std::unique_ptr<JPH::TempAllocatorImpl> m_tempAllocator;
        std::unique_ptr<JPH::JobSystemThreadPool> m_jobSystemThreadPool;
        std::unique_ptr<BroadPhaseLayer> m_broadPhaseLayer;
        std::unique_ptr<ObjectLayerVsBroadPhaseLayerFilter> m_objectVsBroadPhaseLayerFilter;
        std::unique_ptr<ObjectLayerPairFilter> m_objectLayerPairFilter;
        std::unique_ptr<JPH::PhysicsSystem> m_physicsSystem;
        std::unique_ptr<BodyActivationListener> m_bodyActivationListener;
        std::unique_ptr<ContactListener> m_contactListener;

        static constexpr JPH::uint MAX_BODIES = 1024;
        static constexpr JPH::uint NUM_BODY_MUTEXES = 0;
        static constexpr JPH::uint MAX_BODY_PAIRS = 1024;
        static constexpr JPH::uint MAX_CONTACT_CONSTRAINS = 1024;

        static constexpr float FIXED_DELTA_TIME = 1.0f / 60.0f;
        static constexpr int COLLISION_STEPS = 1;
        static constexpr float MAX_ACCOMULATED_TIME = 0.25f;

        float m_accomulator = 0.0f;
        bool m_firstFrame = true;
    };
} // Kita
