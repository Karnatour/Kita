#include "Scene.h"
#include "../Core/Engine.h"

namespace Kita {
    Scene::Scene(): m_camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {
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
    }
} // Kita
