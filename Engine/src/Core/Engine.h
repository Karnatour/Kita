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
        void init(GraphicsAPI API);
        static std::shared_ptr<Engine>& getEngine();

        void initGame();

        void loadGameInstance(std::shared_ptr<IGameInstance> instance);
        void run();
        void stop();

    private:
        void render();
        void update();
        void exit();
        bool m_isRunning = false;
        std::unique_ptr<Window> m_window = nullptr;
        std::unique_ptr<Renderer> m_renderer = nullptr;
        std::shared_ptr<IGameInstance> m_game = nullptr;
    };
}
