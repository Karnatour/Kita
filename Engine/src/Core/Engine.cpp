#include "Engine.h"

#include "../Window/Window.h"
#include "../Events/EventManager.h"

namespace Kita {
    static std::shared_ptr<Engine> m_engineInstance = nullptr;


    Engine::Engine() {
        m_isRunning = true;
    }

    void Engine::init(RenderingAPI API) {
        m_window = std::make_unique<Window>();
        m_window->init();
        m_window->createWindow(600, 600, "Kita");
        m_renderer = std::make_unique<Renderer>(API);

        EventManager::attachEngineEvents();

        KITA_ENGINE_INFO("Engine initialized");
    }

    std::shared_ptr<Engine>& Engine::getEngine() {
        if (m_engineInstance == nullptr) {
            m_engineInstance = std::make_shared<Engine>();
        }
        return m_engineInstance;
    }

    void Engine::initGame() {
        if (m_game->m_initialized == false) {
            m_game->onInit();
            KITA_ENGINE_INFO("Game initialized");
            m_game->m_initialized = true;
        }
    }

    void Engine::loadGameInstance(std::shared_ptr<IGameInstance> instance) {
        m_game = std::move(instance);
        m_isRunning = true;
        run();
    }

    void Engine::run() {
        initGame();
        while (m_isRunning) {
            m_window->poolEvents();

            update();
            m_game->onUpdate();

            render();
            m_game->onRender();

            m_window->swapBuffers();
        }
        m_game->onExit();
        exit();
    }

    void Engine::stop() {
        m_isRunning = false;
    }

    Window& Engine::getWindow(){
        return *m_window;
    }

    Renderer& Engine::getRenderer(){
        return *m_renderer;
    }

    void Engine::update() {
    }

    void Engine::render() {
    }

    void Engine::exit() {
        m_window->exit();
    }
}
