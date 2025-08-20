#include "Sandbox.h"
#include <glm/ext/matrix_transform.hpp>

#include "../../Engine/src/Asset/KAsset.h"

void onSomething(Kita::KeyPressed& event) {
    KITA_DEBUG("LOL");
}

void Sandbox::onInit() {
    m_model = std::make_shared<Kita::Model>();
    m_model->addMesh(std::make_shared<Kita::Mesh>(m_vertices, m_indices, 0));
    m_model->addMaterial(std::make_shared<Kita::Material>("../assets/textures/wood_floor.jpg"));

    m_scene.addModel(*m_model);
    Kita::KAsset::saveToFile(m_model, "../assets/baked/test.kasset");
    m_model = Kita::KAsset::loadFromFile("../assets/baked/test.kasset");

    Kita::EventManager::listenToEvent<Kita::KeyPressed>(onSomething);
}

void Sandbox::onUpdate() {
    m_model->getMaterials().at(0)->getShader()->bind();
    m_model->getMaterials().at(0)->getShader()->setUniformBool("hasTexture", true);
    m_model->getMaterials().at(0)->getShader()->seUniformtInt("texture1", 0);

    auto model = glm::mat4(1.0f);
    model = glm::rotate(model, static_cast<float>(Kita::Time::getElapsedTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    m_model->getMaterials().at(0)->getShader()->setMat4("model", model);

    m_scene.update();
}

void Sandbox::onRender() {
    m_scene.render();
}

void Sandbox::onExit() {
}
