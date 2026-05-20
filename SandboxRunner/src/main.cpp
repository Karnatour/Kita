#include "Sandbox.h"

int main() {
    auto engine = Kita::Engine::getEngine();
    engine->init(Kita::RenderingAPI::OPENGL);

    std::shared_ptr<Sandbox> gameInstance = std::make_shared<Sandbox>();
    engine->loadGameInstance(gameInstance);

    return 0;
}
