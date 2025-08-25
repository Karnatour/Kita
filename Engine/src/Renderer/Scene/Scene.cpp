#include "../../kitapch.h"
#include "Scene.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "../../Core/Engine.h"

namespace Kita {
    Scene::Scene(): m_camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {
        m_cameraUniformBuffer->createBuffer(sizeof(m_camera.getCameraMatrices()), &m_camera.getCameraMatrices());
    }

    void Scene::render() const {
        for (const auto& model : m_entities | std::views::values) {
            Engine::getEngine()->getRenderer().getRendererAPI().render(model);
        }
    }

    void Scene::addEntity(const std::shared_ptr<Entity>& entity) {
        unsigned int id = entity->getID();
        m_entities.emplace(id, entity);
    }

    Camera& Scene::getCamera() {
        return m_camera;
    }

    void Scene::update() {
        m_camera.update();
        updateCameraBuffer();
    }

    std::unordered_map<unsigned int, std::shared_ptr<Entity>>& Scene::getEntities() {
        return m_entities;
    }

    void Scene::updateCameraBuffer() {
        CameraMatrices& matrices = m_camera.getCameraMatrices();

        m_cameraUniformBuffer->bind(0);
        m_cameraUniformBuffer->update(sizeof(matrices), &matrices);

        auto [width, height] = Engine::getEngine()->getWindow().getResolution();
        matrices.projection = glm::perspective(glm::radians(m_camera.getZoom()), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

        matrices.view = m_camera.getViewMatrix();
    }
} // Kita
