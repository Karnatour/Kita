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
        for (const auto& system : m_systems) {
            system->update(*this);
        }
    }

    void Scene::render() {
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
