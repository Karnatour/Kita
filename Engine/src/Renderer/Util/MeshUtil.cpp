#include "../../kitapch.h"

#include "MeshUtil.h"

#include "../../Core/Engine.h"
#include "../Scene/ECS/Entity.h"
#include "../Scene/ECS/Components/MeshComponent.h"
#include "../Scene/ECS/Components/RelationshipComponents.h"
#include "../Scene/ECS/Components/TransformationComponent.h"

namespace Kita {
    std::vector<Mesh*> MeshUtil::getAllMeshesFromEntity(Entity rootEntity) {
        std::vector<Mesh*> meshes;

        if (rootEntity.hasAllComponents<MeshComponent>()) {
            meshes.push_back(&Engine::getEngine()->getAssetManager().getAsset<Mesh>(rootEntity.getComponent<MeshComponent>().meshID));
        }

        if (rootEntity.hasAllComponents<ChildrenComponent>()) {
            for (const auto child : rootEntity.getComponent<ChildrenComponent>().children) {
                std::vector<Mesh*> childMeshes = getAllMeshesFromEntity(Entity(rootEntity.getScene(), child));
                meshes.insert(meshes.end(), childMeshes.begin(), childMeshes.end());
            }
        }

        return meshes;
    }

    std::vector<MeshInstance> MeshUtil::getAllMeshInstancesFromEntity(const Entity rootEntity) {
        return getAllMeshInstancesFromEntityRecursive(rootEntity, glm::mat4(1.0f));
    }

    std::vector<MeshInstance> MeshUtil::getAllMeshInstancesFromEntityRecursive(const Entity entity, const glm::mat4& accumulated) {
        std::vector<MeshInstance> meshInstances;

        const glm::mat4 localModel = entity.hasAllComponents<TransformationComponent>() ? entity.getComponent<TransformationComponent>().localModel : glm::mat4(1.0f);
        const glm::mat4 relativeTransform = accumulated * localModel;

        if (entity.hasAllComponents<MeshComponent>()) {
            Mesh& mesh = Engine::getEngine()->getAssetManager().getAsset<Mesh>(entity.getComponent<MeshComponent>().meshID);
            meshInstances.emplace_back(&mesh, relativeTransform);
        }

        if (entity.hasAllComponents<ChildrenComponent>()) {
            for (const auto child : entity.getComponent<ChildrenComponent>().children) {
                auto childInstances = getAllMeshInstancesFromEntityRecursive(Entity(entity.getScene(), child), relativeTransform);
                meshInstances.insert(meshInstances.end(), childInstances.begin(), childInstances.end());
            }
        }

        return meshInstances;
    }
} // Kita
