#include "Engine.h"

#include "kitapch.h"
#include "Window/Window.h"
#include "Events/EventManager.h"

namespace Kita {
    static std::shared_ptr<Engine> m_engineInstance = nullptr;

    Engine::Engine() {
        m_isRunning = true;
    }

    void Engine::init() {
        m_engineInstance = std::make_shared<Engine>();
        Window::setErrorCallbackFun();
        EventManager::attachEngineEvents();
    }

    std::shared_ptr<Engine>& Engine::getEngine() {
        return m_engineInstance;
    }

    void Engine::loadGameInstance(std::shared_ptr<IGameInstance> instance) {
        m_game = std::move(instance);
        m_isRunning = true;
        run();
    }

    void Engine::run() {
        while (m_isRunning) {
            if (m_game) {
                if (m_game->m_initialized == false) {
                    m_game->onInit();
                    KITA_ENGINE_INFO("Game initialized");
                    m_game->m_initialized = true;
                }

                m_game->onUpdate();
                m_game->onRender();
            }
        }
    }

    void Engine::exit() {
        m_isRunning = false;
    }
}
