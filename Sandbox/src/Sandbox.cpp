#include "Sandbox.h"
#include <glm/ext/matrix_transform.hpp>

void onSomething(Kita::KeyPressed& event) {
    KITA_DEBUG("LOL");
}

void Sandbox::onInit() {
    m_mesh = std::make_shared<Kita::Mesh>(m_vertices, m_indices, "../assets/textures/wood_floor.jpg");
    m_scene.addMesh(*m_mesh);
    Kita::EventManager::listenToEvent<Kita::KeyPressed>(onSomething);
}

void Sandbox::onUpdate() {
    m_mesh->getShader()->bind();
    m_mesh->getShader()->seUniformtInt("texture1", 0);

    auto model = glm::mat4(1.0f);
    model = glm::rotate(model, static_cast<float>(Kita::Time::getElapsedTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    m_mesh->getShader()->setMat4("model", model);

    m_scene.update();
}

void Sandbox::onRender() {
    m_scene.render();
}

void Sandbox::onExit() {
}
