#include "KitaEngine/Kita.h"
#include <vector>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

void onSomething(Kita::KeyPressed& event) {
    KITA_DEBUG("LOL");
}

struct CameraMat {
    glm::mat4 view;
    glm::mat4 projection;
};

class Sandbox : public IGameInstance {
public:
    void onInit() override {
        m_mesh = std::make_shared<Kita::Mesh>(m_vertices, m_indices, "../assets/textures/wood_floor.jpg");
        m_scene.addMesh(*m_mesh);
        Kita::EventManager::listenToEvent<Kita::KeyPressed>(onSomething);
        uniformBuffer->createBuffer(sizeof(m_cameraMat), &m_cameraMat);
    }

    void onUpdate() override {
        m_mesh->getShader()->bind();
        m_mesh->getShader()->seUniformtInt("texture1", 0);
        uniformBuffer->bind(0);
        uniformBuffer->update(sizeof(m_cameraMat), &m_cameraMat);

        auto res = Kita::Engine::getEngine()->getWindow().getResolution();
        m_cameraMat.projection = glm::perspective(glm::radians(m_scene.getCamera().getZoom()), (float)res.first / (float)res.second, 0.1f, 100.0f);

        m_cameraMat.view = m_scene.getCamera().getViewMatrix();


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)Kita::Time::getElapsedTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        m_mesh->getShader()->setMat4("model", model);
        m_scene.update();
    }

    void onRender() override {
        m_scene.render();
    }

    void onExit() override {
    }

private:
    CameraMat m_cameraMat = {};
    std::shared_ptr<Kita::UniformBuffer> uniformBuffer = Kita::UniformBuffer::createPtr();

    std::vector<Kita::Vertex> m_vertices = Kita::Cube::vertices;
    std::vector<unsigned int> m_indices = Kita::Cube::indices;
    std::shared_ptr<Kita::Mesh> m_mesh;
    Kita::Scene m_scene;
};

int main() {
    auto engine = Kita::Engine::getEngine();
    engine->init(Kita::RenderingAPI::OPENGL);

    std::shared_ptr<Sandbox> gameInstance = std::make_shared<Sandbox>();
    engine->loadGameInstance(gameInstance);

    return 0;
}
