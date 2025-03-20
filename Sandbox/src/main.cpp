#include "KitaEngine/Kita.h"
#include <vector>
class Sandbox : public IGameInstance {
public:
    void onInit() override {

    }

    void onUpdate() override {
    }

    void onRender() override {
    }

    void onExit() override {
    }
};

int main() {
    auto engine = Kita::Engine::getEngine();
    engine->init(Kita::GraphicsAPI::OPENGL);

    std::shared_ptr<Sandbox> gameInstance = std::make_shared<Sandbox>();
    engine->loadGameInstance(gameInstance);

    return 0;
}
