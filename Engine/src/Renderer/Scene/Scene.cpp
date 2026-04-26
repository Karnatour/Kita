#include "../../kitapch.h"
#include "Scene.h"

#include "../../Core/Engine.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/SkyboxComponent.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/GeometrySystem.h"
#include "ECS/Systems/LightShadowSystem.h"
#include "ECS/Systems/PostProcessingSystem.h"

namespace Kita {
    void Scene::addDefaultSystems() {
        m_systems.emplace_back(std::make_unique<CameraSystem>());
        m_systems.emplace_back(std::make_unique<LightShadowSystem>());
        m_systems.emplace_back(std::make_unique<GeometrySystem>());
        m_systems.emplace_back(std::make_unique<PostProcessingSystem>());

        Entity camera = createEntity();
        camera.addComponent<CameraComponent>();
        camera.addComponent<ActiveCamera>();
        Entity skybox = createEntity();
        skybox.addComponent<SkyboxComponent>(SkyboxComponent{
            .skyboxID = Engine::getEngine()->getAssetManager().createAsset<Texture>("defaultSkybox.hdr", {}, Texture::TextureType::SKYBOX, std::nullopt)
        });
    }

    void Scene::update() {
        for (const auto& system : m_systems) {
            system->update(*this);
        }
    }

    void Scene::render() {
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
} // Kita
