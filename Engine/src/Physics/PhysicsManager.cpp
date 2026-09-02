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
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include "../Core/Time.h"
#include "../Renderer/Scene/ECS/Components/TransformationComponent.h"
#include "../Renderer/Util/MeshUtil.h"
#include "../Renderer/Util/TransformationUtil.h"

namespace Kita {
    JPH::BodyID PhysicsManager::createBody(const JPH::BodyCreationSettings& creationSettings, const JPH::EActivation activate) {
        return m_physicsSystem->GetBodyInterface().CreateAndAddBody(creationSettings, activate);
    }

    JPH::BodyID PhysicsManager::createBody(const Entity rootEntity, const JPH::EMotionType motionType, const PhysicsLayers::Layers layer, const JPH::EActivation activate) {
        const glm::mat4 rootWorldMatrix = rootEntity.getComponent<TransformationComponent>().worldModel;

        const JPH::Shape::ShapeResult result = createCompoundShape(rootEntity, rootWorldMatrix, motionType);

        if (result.HasError()) {
            KITA_ENGINE_ERROR("[PhysicsManager] Unable to create Shape, error: {}", result.GetError());
            return JPH::BodyID(JPH::BodyID::cInvalidBodyID);
        }

        const auto decomposedRoot = TransformationUtil::decompose(rootWorldMatrix);
        const JPH::RVec3 rootPosition(decomposedRoot.position.x, decomposedRoot.position.y, decomposedRoot.position.z);
        const JPH::Quat rootRotation(decomposedRoot.rotation.x, decomposedRoot.rotation.y, decomposedRoot.rotation.z, decomposedRoot.rotation.w);

        auto creationSettings = JPH::BodyCreationSettings(result.Get(), rootPosition, rootRotation, motionType, layer);

        return createBody(creationSettings, activate);
    }

    JPH::BodyID PhysicsManager::changeMotionType(const Entity rootEntity, const JPH::BodyID bodyID, const JPH::EMotionType motionType, const PhysicsLayers::Layers layer, const JPH::EActivation activate) {
        if (m_physicsSystem->GetBodyInterface().GetMotionType(bodyID) == motionType) {
            return bodyID;
        }

        const JPH::BodyID newBodyID = createBody(rootEntity, motionType, layer, activate);
        if (newBodyID.IsInvalid()) {
            KITA_ENGINE_ERROR("[PhysicsManager] Failed to create body for new motion type, keeping existing body");
            return bodyID;
        }

        m_physicsSystem->GetBodyInterface().RemoveBody(bodyID);
        m_physicsSystem->GetBodyInterface().DestroyBody(bodyID);

        return newBodyID;
    }

    void PhysicsManager::changePosition(const JPH::BodyID id, const glm::vec3 position) {
        m_physicsSystem->GetBodyInterface().SetPosition(id, JPH::Vec3Arg(position.x, position.y, position.z), JPH::EActivation::Activate);
    }

    glm::vec3 PhysicsManager::getPosition(const JPH::BodyID id) const {
        const JPH::RVec3 joltPos = m_physicsSystem->GetBodyInterface().GetPosition(id);
        return glm::vec3(joltPos.GetX(), joltPos.GetY(), joltPos.GetZ());
    }

    glm::mat4 PhysicsManager::getModelMatrix(const JPH::BodyID id) const {
        const JPH::RMat44 joltMat = m_physicsSystem->GetBodyInterface().GetWorldTransform(id);

        glm::mat4 model;
        for (int col = 0; col < 4; ++col) {
            const JPH::Vec4 c = joltMat.GetColumn4(col);
            model[col] = glm::vec4(c.GetX(), c.GetY(), c.GetZ(), c.GetW());
        }

        return model;
    }

    void PhysicsManager::removeBody(const JPH::BodyID id) {
        m_physicsSystem->GetBodyInterface().RemoveBody(id);
    }

    void PhysicsManager::destroyBody(const JPH::BodyID id) {
        m_physicsSystem->GetBodyInterface().DestroyBody(id);
    }

    bool PhysicsManager::isBodyActive(const JPH::BodyID id) const {
        return m_physicsSystem->GetBodyInterface().IsActive(id);
    }

    void PhysicsManager::activateBody(const JPH::BodyID id) {
        m_physicsSystem->GetBodyInterface().ActivateBody(id);
    }

    void PhysicsManager::deactivateBody(const JPH::BodyID id) {
        m_physicsSystem->GetBodyInterface().DeactivateBody(id);
    }

    void PhysicsManager::init() {
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

    void PhysicsManager::update() {
        if (m_firstFrame) {
            m_physicsSystem->OptimizeBroadPhase();
            m_firstFrame = false;
        }

        m_accomulator = m_accomulator + static_cast<float>(Time::getDeltaTime());
        m_accomulator = std::min(m_accomulator, MAX_ACCOMULATED_TIME);

        while (m_accomulator >= FIXED_DELTA_TIME) {
            m_physicsSystem->Update(m_accomulator, COLLISION_STEPS, m_tempAllocator.get(), m_jobSystemThreadPool.get());
            m_accomulator = m_accomulator - FIXED_DELTA_TIME;
        }
    }

    void PhysicsManager::exit() {
        JPH::UnregisterTypes();

        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    void PhysicsManager::TraceImpl(const char* inFMT, ...) {
        va_list list;
        va_start(list, inFMT);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), inFMT, list);
        va_end(list);

        // Print to the TTY
        KITA_ENGINE_DEBUG("[PhysicsManager] {}", buffer);
    }

    bool PhysicsManager::AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine) {
        KITA_ENGINE_ERROR("{} : {} : ({}) {}", inFile, inLine, inExpression, (inMessage != nullptr ? inMessage : ""));

        return true;
    }

    JPH::ShapeSettings::ShapeResult PhysicsManager::createMeshShapeSettings(const Mesh& mesh) {
        if (mesh.getIndexBuffer() == nullptr) {
            JPH::TriangleList triangleList;

            auto& vertices = mesh.getVertexBuffer().getVertices();
            KITA_ENGINE_ASSERT(!vertices.empty(), "Mesh has no vertices");

            triangleList.reserve(vertices.size() / 3);

            KITA_ENGINE_ASSERT(vertices.size() % 3 == 0, "Vertices must be triangulated");

            for (size_t i = 0; i < vertices.size(); i = i + 3) {
                triangleList.push_back(JPH::Triangle(
                    JPH::Float3(vertices[i + 0].position.x, vertices[i + 0].position.y, vertices[i + 0].position.z),
                    JPH::Float3(vertices[i + 1].position.x, vertices[i + 1].position.y, vertices[i + 1].position.z),
                    JPH::Float3(vertices[i + 2].position.x, vertices[i + 2].position.y, vertices[i + 2].position.z)));
            }

            return JPH::MeshShapeSettings(triangleList).Create();
        }

        JPH::VertexList vertexList;
        JPH::IndexedTriangleList triangleList;

        auto& vertices = mesh.getVertexBuffer().getVertices();
        KITA_ENGINE_ASSERT(!vertices.empty(), "Mesh has no vertices");

        vertexList.reserve(vertices.size() / 3);

        for (const auto& vertex : vertices) {
            vertexList.push_back(JPH::Float3(vertex.position.x, vertex.position.y, vertex.position.z));
        }

        auto& indices = mesh.getIndexBuffer()->getIndices();
        triangleList.reserve(indices.size() / 3);

        KITA_ENGINE_ASSERT(indices.size() % 3 == 0, "Indices must be triangulated");

        for (size_t i = 0; i < indices.size(); i = i + 3) {
            triangleList.push_back(JPH::IndexedTriangle(indices[i + 0], indices[i + 1], indices[i + 2]));
        }

        return JPH::MeshShapeSettings(vertexList, triangleList).Create();
    }

    JPH::ShapeSettings::ShapeResult PhysicsManager::createConvexHullShape(const Mesh& mesh) {
        JPH::Array<JPH::Vec3> points;
        auto& vertices = mesh.getVertexBuffer().getVertices();
        points.reserve(vertices.size());
        for (const auto& vertex : vertices) {
            points.push_back(JPH::Vec3(vertex.position.x, vertex.position.y, vertex.position.z));
        }

        return JPH::ConvexHullShapeSettings(points).Create();
    }

    JPH::Shape::ShapeResult PhysicsManager::createCompoundShape(Entity rootEntity, const glm::mat4& rootWorldMatrix, JPH::EMotionType motionType) {
        JPH::StaticCompoundShapeSettings compoundSettings;

        const glm::mat4 invRoot = glm::inverse(rootWorldMatrix);

        for (const auto& [mesh, worldTransform] : MeshUtil::getAllMeshInstancesFromEntity(rootEntity)) {
            JPH::Shape::ShapeResult result;

            if (motionType == JPH::EMotionType::Static) {
                result = createMeshShapeSettings(*mesh);
            } else if (motionType == JPH::EMotionType::Dynamic) {
                result = createConvexHullShape(*mesh);
            }

            const glm::mat4 relativeTransform = invRoot * worldTransform;
            const TransformationUtil::DecomposedTransform decomposed = TransformationUtil::decompose(relativeTransform);

            const auto position = JPH::RVec3(decomposed.position.x, decomposed.position.y, decomposed.position.z);
            const auto rotation = JPH::Quat(decomposed.rotation.x, decomposed.rotation.y, decomposed.rotation.z, decomposed.rotation.w);

            JPH::ShapeRefC subShape = result.Get();

            constexpr float kScaleEpsilon = 1e-4f;
            if (glm::any(glm::greaterThan(glm::abs(decomposed.scale - glm::vec3(1.0f)), glm::vec3(kScaleEpsilon)))) {
                JPH::ScaledShapeSettings scaledSettings(subShape, JPH::Vec3(decomposed.scale.x, decomposed.scale.y, decomposed.scale.z));
                auto scaledResult = scaledSettings.Create();
                if (scaledResult.HasError()) {
                    KITA_ENGINE_ERROR("[PhysicsManager] Failed to scale sub-shape: {}", scaledResult.GetError());
                    continue;
                }
                subShape = scaledResult.Get();
            }

            compoundSettings.AddShape(position, rotation, subShape);
        }

        return compoundSettings.Create();
    }
} // Kita
