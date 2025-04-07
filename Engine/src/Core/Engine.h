#pragma once
#include "DllTemplate.h"
#include "../instances/IGameInstance.h"
#include "../Window/Window.h"
#include "../Renderer/Renderer.h"
#include <memory>


namespace Kita {
    class KITAENGINE_API Engine {
    public:
        Engine();
        void init(RenderingAPI API);
        static std::shared_ptr<Engine>& getEngine();

        void initGame();

        void loadGameInstance(std::shared_ptr<IGameInstance> instance);
        void run();
        void stop();

        Window& getWindow() const;
        Renderer& getRenderer() const;
    private:
        void render();
        void update();
        void exit();
        bool m_isRunning = false;
        std::unique_ptr<Window> m_window;
        std::unique_ptr<Renderer> m_renderer;
        std::shared_ptr<IGameInstance> m_game;
    };
}
