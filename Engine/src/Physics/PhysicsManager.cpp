#include "../kitapch.h"
#include "PhysicsManager.h"

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include "../Core/Time.h"

namespace Kita
{
    JPH::BodyID PhysicsManager::createBody(const JPH::BodyCreationSettings& creationSettings, const bool activate)
    {
        return m_physicsSystem->GetBodyInterface().CreateAndAddBody(creationSettings, activate ? JPH::EActivation::Activate : JPH::EActivation::DontActivate);
    }

    void PhysicsManager::removeBody(const JPH::BodyID id)
    {
        m_physicsSystem->GetBodyInterface().RemoveBody(id);
    }

    void PhysicsManager::destroyBody(const JPH::BodyID id)
    {
        m_physicsSystem->GetBodyInterface().DestroyBody(id);
    }

    bool PhysicsManager::isBodyActive(const JPH::BodyID id) const
    {
        return m_physicsSystem->GetBodyInterface().IsActive(id);
    }

    void PhysicsManager::activateBody(const JPH::BodyID id)
    {
        m_physicsSystem->GetBodyInterface().ActivateBody(id);
    }

    void PhysicsManager::deactivateBody(const JPH::BodyID id)
    {
        m_physicsSystem->GetBodyInterface().DeactivateBody(id);
    }

    void PhysicsManager::init()
    {
        JPH::RegisterDefaultAllocator();

        JPH::Trace = TraceImpl;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl);

        JPH::Factory::sInstance = new JPH::Factory();

        JPH::RegisterTypes();

        m_tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
        m_jobSystemThreadPool = std::make_unique<JPH::JobSystemThreadPool>(
            JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
        m_broadPhaseLayer = std::make_unique<BroadPhaseLayer>();
        m_objectVsBroadPhaseLayerFilter = std::make_unique<ObjectLayerVsBroadPhaseLayerFilter>();
        m_objectLayerPairFilter = std::make_unique<ObjectLayerPairFilter>();

        m_physicsSystem = std::make_unique<JPH::PhysicsSystem>();
        m_physicsSystem->Init(MAX_BODIES, NUM_BODY_MUTEXES, MAX_BODY_PAIRS, MAX_CONTACT_CONSTRAINS, *m_broadPhaseLayer,
                              *m_objectVsBroadPhaseLayerFilter, *m_objectLayerPairFilter);

        m_bodyActivationListener = std::make_unique<BodyActivationListener>();
        m_physicsSystem->SetBodyActivationListener(m_bodyActivationListener.get());

        m_contactListener = std::make_unique<ContactListener>();
        m_physicsSystem->SetContactListener(m_contactListener.get());
    }

    void PhysicsManager::update()
    {
        if (m_firstFrame)
        {
            m_physicsSystem->OptimizeBroadPhase();
            m_firstFrame = false;
        }

        m_accomulator = m_accomulator + static_cast<float>(Time::getDeltaTime());
        m_accomulator = std::min(m_accomulator, MAX_ACCOMULATED_TIME);

        while (m_accomulator >= FIXED_DELTA_TIME)
        {
            m_physicsSystem->Update(m_accomulator, COLLISION_STEPS, m_tempAllocator.get(), m_jobSystemThreadPool.get());
            m_accomulator = m_accomulator - FIXED_DELTA_TIME;
        }
    }

    void PhysicsManager::exit()
    {
        JPH::UnregisterTypes();

        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    void PhysicsManager::TraceImpl(const char* inFMT, ...)
    {
        va_list list;
        va_start(list, inFMT);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), inFMT, list);
        va_end(list);

        // Print to the TTY
        KITA_ENGINE_DEBUG("[PhysicsManager] {}", buffer);
    }

    bool PhysicsManager::AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile,
                                          JPH::uint inLine)
    {
        KITA_ENGINE_ERROR("{} : {} : ({}) {}", inFile, inLine, inExpression, (inMessage != nullptr ? inMessage : ""));

        return true;
    }
} // Kita
