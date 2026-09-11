#include "Scene.h"

#include "../../Core/Engine.h"
#include "ECS/EntityBuilder.h"
#include "ECS/Components/PostProcessingComponent.h"
#include "ECS/Components/SceneSettingsComponent.h"
#include "ECS/Components/SkyboxComponent.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/GeometrySystem.h"
#include "ECS/Systems/LightShadowSystem.h"
#include "ECS/Systems/PhysicsSystem.h"
#include "ECS/Systems/PostProcessingSystem.h"
#include "ECS/Systems/SkyboxSystem.h"

namespace Kita {
    void Scene::addDefaultSystems() {
        m_systems.emplace_back(std::make_unique<CameraSystem>());
        m_systems.emplace_back(std::make_unique<PhysicsSystem>(*this));
        m_systems.emplace_back(std::make_unique<LightShadowSystem>());
        m_systems.emplace_back(std::make_unique<GeometrySystem>());
        m_systems.emplace_back(std::make_unique<SkyboxSystem>());
        m_systems.emplace_back(std::make_unique<PostProcessingSystem>());

        m_camera = EntityBuilder::createActiveCamera(*this).getEnttEntityID();
        Entity skybox = createEntity();
        skybox.addComponent<SkyboxComponent>(SkyboxComponent{
            .skyboxID = Engine::getEngine()->getAssetManager().createAsset<Texture>("DefaultSkybox.hdr", {}, Texture::TextureType::SKYBOX, std::nullopt)
        });
        Entity postProcessing = createEntity();
        postProcessing.addComponent<PostProcessingComponent>();
        Entity sceneSettings = createEntity();
        sceneSettings.addComponent<SceneSettingsComponent>();
    }

    void Scene::update() {
        KITA_ENGINE_PROFILE("Scene update");
        for (const auto& system : m_systems) {
            system->update(*this);
        }
    }

    void Scene::render() {
        KITA_ENGINE_PROFILE("Scene render");
        //TODO dirty flag ?
        std::ranges::sort(m_systems, [](auto& a, auto& b) {
            return a->getOrder() < b->getOrder();
        });

        for (const auto& system : m_systems) {
            system->render(*this);
        }
    }

    Entity Scene::createEntity() {
        return Entity(this);
    }

    entt::entity Scene::getCameraEntity() const {
        return m_camera;
    }
} // Kita
