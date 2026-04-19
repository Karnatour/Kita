#include "../../kitapch.h"
#include "Scene.h"

#include "../../Core/Engine.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/LightShadowSystem.h"

namespace Kita {
    /*m_skyboxEntity = std::make_shared<SkyboxEntity>("defaultSkybox.hdr");*/

    void Scene::addDefaultSystems() {
        m_systems.emplace_back(std::make_unique<CameraSystem>());
        m_systems.emplace_back(std::make_unique<LightShadowSystem>());
        //m_systems.emplace_back(std::make_unique<RenderSystem>());
    }

    void Scene::update() {
        for (auto& system : m_systems) {
            system->update(*this);
        }
    }

    void Scene::render() {
        for (auto& system : m_systems) {
            system->render(*this);
        }
    }

    Entity Scene::createEntity() {
        return Entity(this);
    }

} // Kita
