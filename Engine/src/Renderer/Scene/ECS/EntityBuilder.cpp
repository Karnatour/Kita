#include "../../../kitapch.h"
#include "EntityBuilder.h"

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
        playerEntity.addComponent<CameraComponent>(CameraComponent{.properties = {.position = position,.ignorePosition = true}});
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
} // Kita
