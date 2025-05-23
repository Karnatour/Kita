#include "Scene.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "../Core/Engine.h"

namespace Kita {
    Scene::Scene(): m_camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {
        m_uniformBuffer->createBuffer(sizeof(m_camera.getCameraMatrices()), &m_camera.getCameraMatrices());
    }

    void Scene::render() const {
        for (auto mesh : m_meshes | std::views::values) {
            Engine::getEngine()->getRenderer().getRendererAPI().render(mesh);
        }
    }

    void Scene::addMesh(Mesh&& mesh) {
        m_meshes.insert({mesh.getID(), std::move(mesh)});
    }

    void Scene::addMesh(Mesh& mesh) {
        m_meshes.insert({mesh.getID(), mesh});
    }

    Camera& Scene::getCamera() {
        return m_camera;
    }

    void Scene::update() {
        m_camera.update();
        updateCameraBuffer();
    }

    void Scene::updateCameraBuffer() {
        CameraMatrices& matrices = m_camera.getCameraMatrices();

        m_uniformBuffer->bind(0);
        m_uniformBuffer->update(sizeof(matrices), &matrices);

        auto res = Engine::getEngine()->getWindow().getResolution();
        matrices.projection = glm::perspective(glm::radians(m_camera.getZoom()), static_cast<float>(res.first) / static_cast<float>(res.second), 0.1f, 100.0f);

        matrices.view = m_camera.getViewMatrix();
    }
} // Kita
