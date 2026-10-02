#include "EntityBuilder.h"

#include <Jolt/Physics/EActivation.h>

#include "../../../Assets/AssetImporter.h"
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

    Entity EntityBuilder::createVehicle(Scene& scene, const std::string& path, const glm::vec3 position) {
        auto carEntity = scene.createEntity();
        carEntity = AssetImporter::importModel(path, scene, true).value();
        carEntity.addComponent<VehicleComponent>(VehicleComponent{.properties = {.position = position, .fourWheelDrive = true}});
        carEntity.addComponent<CameraComponent>(CameraComponent{.properties = {.position = position, .ignorePosition = true, .ignoreEulerAnglesUpdate = true    }});
        carEntity.addComponent<PhysicsComponent>();

        bool wheelsFound = false;
        for (const auto child : carEntity.getComponent<ChildrenComponent>().children) {
            if (auto wheelsEntity = Entity(&scene, child); wheelsEntity.getComponent<NameComponent>().name == "KITA_Wheels") {
                wheelsFound = true;
                for (auto wheelID : wheelsEntity.getComponent<ChildrenComponent>().children) {
                    auto wheelEntity = Entity(&scene, wheelID);
                    const std::string& wheelName = wheelEntity.getComponent<NameComponent>().name;
                    if (wheelName == "KITA_Wheel_Front_L") {
                        carEntity.getComponent<VehicleComponent>().properties.frontLeftWheelEnttID = wheelID;
                    } else if (wheelName == "KITA_Wheel_Front_R") {
                        carEntity.getComponent<VehicleComponent>().properties.frontRightWheelEnttID = wheelID;
                    } else if (wheelName == "KITA_Wheel_Rear_L") {
                        carEntity.getComponent<VehicleComponent>().properties.rearLeftWheelEnttID = wheelID;
                    } else if (wheelName == "KITA_Wheel_Rear_R") {
                        carEntity.getComponent<VehicleComponent>().properties.rearRightWheelEnttID = wheelID;
                    } else {
                        wheelsFound = false;
                    }
                }
            }
        }
        KITA_ENGINE_ASSERT(wheelsFound, std::format("[EntityBuilder] Wheels were not found for vehicle: {}", path));

        return carEntity;
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
        // Root has no parent, so world == local
        rootEntity.getComponent<TransformationComponent>().worldModel = rootEntity.getComponent<TransformationComponent>().localModel;
        return rootEntity;
    }

    Entity EntityBuilder::createNodeRenderEntity(Scene& scene, const std::optional<std::string>& name, const std::optional<TransformationComponent>& transformationComponent) {
        auto nodeEntity = scene.createEntity();
        nodeEntity.addComponent<ChildrenComponent>();
        nodeEntity.addComponent<TransformationComponent>(transformationComponent.value_or((TransformationComponent{})));
        nodeEntity.addComponent<RenderInShadowPass>();
        nodeEntity.addComponent<RenderInMainPass>();
        if (name.has_value()) {
            KITA_ENGINE_DEBUG("[KAsset] Created node entity with name: {}", name.value());
        }
        nodeEntity.addComponent<NameComponent>(NameComponent{.name = name.value_or("Unnamed node")});
        return nodeEntity;
    }


    Entity EntityBuilder::createChildRenderEntity(Scene& scene, const AssetManager::AssetID meshAssetID, const AssetManager::AssetID shaderAssetID, const AssetManager::AssetID albedoTextureAssetID) {
        auto childEntity = scene.createEntity();
        childEntity.addComponent<MeshComponent>(MeshComponent{.meshID = meshAssetID});
        childEntity.addComponent<MaterialComponent>(MaterialComponent{.shaderID = shaderAssetID,.albedoTextureID = albedoTextureAssetID});
        return childEntity;
    }

    bool EntityBuilder::finalizeStaticBody(Entity rootEntity, const PhysicsLayers::Layers layer, const JPH::EActivation activate) {
        const JPH::BodyID bodyID = Engine::getEngine()->getPhysicsManager().createBody(rootEntity, JPH::EMotionType::Static, layer, activate);
        if (bodyID.IsInvalid()) {
            KITA_ENGINE_ERROR("[EntityBuilder] Failed to create static physics body for entity '{}'", rootEntity.hasAllComponents<NameComponent>() ? rootEntity.getComponent<NameComponent>().name : "Unnamed");
            return false;
        }
        rootEntity.addComponent<PhysicsComponent>(PhysicsComponent{.bodyID = bodyID});
        return true;
    }
} // Kita
