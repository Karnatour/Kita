#pragma once
#include "IGameInstance.h"
#include "DllTemplate.h"
#include "Window/Window.h"
#include <memory>

namespace Kita {
    class KITAENGINE_API Engine {
    public:
        Engine();
        void init();
        static std::shared_ptr<Engine>& getEngine();

        void initGame();

        void loadGameInstance(std::shared_ptr<IGameInstance> instance);
        void run();
        void stop();

        Window m_window;

    private:
        void render();
        void update();
        void exit();
        bool m_isRunning = false;
        std::shared_ptr<IGameInstance> m_game = nullptr;
    };
}
