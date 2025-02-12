#include "Engine.h"

#include "Logger.h"

namespace Kita {

    bool Engine::isRunning = false;
    std::shared_ptr<Engine> Engine::engineInstance;
    std::shared_ptr<IGameInstance> Engine::gameInstance;

    Engine::Engine() {
        Logger::init();
        isRunning = true;
    }

    Engine::~Engine() {
    }

    std::shared_ptr<Engine>& Engine::getEngine() {
        if (!engineInstance) {
            engineInstance = std::make_shared<Engine>();
        }
        return engineInstance;
    }

    void Engine::loadGameInstance(std::shared_ptr<IGameInstance> instance) {
        gameInstance = std::move(instance);
    }

    void Engine::run() {
        while (isRunning) {
            if (gameInstance) {
                gameInstance->onUpdate();
                gameInstance->onRender();
            }
        }
    }

    void Engine::exit() {
        isRunning = false;
    }
}
