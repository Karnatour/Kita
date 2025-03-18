#include "KitaEngine/Kita.h"

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
    engine->init();

    std::shared_ptr<Sandbox> gameInstance = std::make_shared<Sandbox>();
    engine->loadGameInstance(gameInstance);

    return 0;
}
