#include "../../../kitapch.h"
#include "EntityBuilder.h"

#include <Jolt/Physics/EActivation.h>

#include "../../../Core/Engine.h"
#include "../../../Physics/PhysicsLayers.h"
#include "../../Util/CameraUtil.h"
#include "Components/Components.h"

namespace Kita {
    Entity EntityBuilder::createDirectionalLight(Scene& scene, const bool castShadows) {
        auto lightEntity = scene.createEntity();
        lightEntity.addComponent<LightComponent>(LightComponent{.properties = LightProperties{.direction = glm::vec3(-0.1f, -1.0f, 0.1f), .lightType = LightType::DIRECTIONAL}});
        lightEntity.addComponent<DirectionalShadowComponent>();

        if (castShadows) {
            lightEntity.addComponent<CastsShadows>();
        }

        return lightEntity;
    }

    Entity EntityBuilder::createPlayerCharacter(Scene& scene, const std::optional<std::string>& name, const glm::vec3 position) {
        auto playerEntity = scene.createEntity();
        playerEntity.addComponent<PlayerCharacterComponent>(PlayerCharacterComponent{.properties = {.position = position}});
        playerEntity.addComponent<CameraComponent>(CameraComponent{.properties = {.position = position, .ignorePosition = true}});
        playerEntity.addComponent<NameComponent>(NameComponent{.name = name.value_or("Unnamed Player")});

        return playerEntity;
    }

    Entity EntityBuilder::createActiveCamera(Scene& scene) {
        auto cameraEntity = scene.createEntity();
        cameraEntity.addComponent<CameraComponent>();
        if (!CameraUtil::activeCameraExists(scene)) {
            cameraEntity.addComponent<ActiveCamera>();
        } else {
            KITA_ENGINE_WARN("[EntityBuilder] Tried to create ActiveCamera while other camera is already active");
        }
        return cameraEntity;
    }

    Entity EntityBuilder::createRootRenderEntity(Scene& scene, const std::optional<std::string>& name, const std::optional<TransformationComponent>& transformationComponent) {
        auto rootEntity = createNodeRenderEntity(scene, name, transformationComponent);
        rootEntity.addComponent<PhysicsComponent>(PhysicsComponent{.bodyID = Engine::getEngine()->getPhysicsManager().createBody(rootEntity, JPH::EMotionType::Static, PhysicsLayers::STATIC, JPH::EActivation::Activate)});
        return rootEntity;
    }

    Entity EntityBuilder::createNodeRenderEntity(Scene& scene, const std::optional<std::string>& name, const std::optional<TransformationComponent>& transformationComponent) {
        auto nodeEntity = scene.createEntity();
        nodeEntity.addComponent<ChildrenComponent>();
        nodeEntity.addComponent<TransformationComponent>(transformationComponent.value_or((TransformationComponent{})));
        nodeEntity.addComponent<RenderInShadowPass>();
        nodeEntity.addComponent<RenderInMainPass>();
        nodeEntity.addComponent<NameComponent>(NameComponent{.name = name.value_or("Unnamed Player")});
        return nodeEntity;
    }


    Entity EntityBuilder::createChildRenderEntity(Scene& scene, const AssetManager::AssetID meshAssetID, const AssetManager::AssetID shaderAssetID) {
        auto childEntity = scene.createEntity();
        childEntity.addComponent<MeshComponent>(MeshComponent{.meshID = meshAssetID});
        childEntity.addComponent<MaterialComponent>(MaterialComponent{.shaderID = shaderAssetID});
        return childEntity;
    }

    bool EntityBuilder::finalizeStaticBody(Entity rootEntity, PhysicsLayers::Layers layer, JPH::EActivation activate) {
        const JPH::BodyID bodyID = Engine::getEngine()->getPhysicsManager().createBody(rootEntity, JPH::EMotionType::Static, layer, activate);
        if (bodyID.IsInvalid()) {
            KITA_ENGINE_ERROR("[EntityBuilder] Failed to create static physics body for entity '{}'", rootEntity.hasAllComponents<NameComponent>() ? rootEntity.getComponent<NameComponent>().name : "Unnamed");
            return false;
        }
        rootEntity.addComponent<PhysicsComponent>(PhysicsComponent{.bodyID = bodyID});
        return true;
    }
} // Kita
