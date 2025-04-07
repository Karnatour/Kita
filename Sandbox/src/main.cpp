#include "KitaEngine/Kita.h"
#include <vector>
#include <windows.h>

class Sandbox : public IGameInstance {
public:
    void onInit() override {
        mesh = std::make_unique<Kita::Mesh>(vertices, indices);
    }

    void onUpdate() override {
    }

    void onRender() override {
        Kita::Engine::getEngine()->getRenderer().getRendererAPI().render(*mesh);
    }

    void onExit() override {
    }

private:
    std::vector<Kita::Vertex> vertices = {
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // top right
        {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // bottom right
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // bottom left
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // top left
    };

    std::vector<unsigned int> indices = {
        0, 1, 3, // first Triangle
        1, 2, 3 // second Triangle
    };
    std::unique_ptr<Kita::Mesh> mesh;
};

int main() {
    auto engine = Kita::Engine::getEngine();
    engine->init(Kita::RenderingAPI::OPENGL);

    std::shared_ptr<Sandbox> gameInstance = std::make_shared<Sandbox>();
    engine->loadGameInstance(gameInstance);

    return 0;
}
