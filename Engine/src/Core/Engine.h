#pragma once
#include "DllTemplate.h"
#include "../instances/IGameInstance.h"
#include "../Window/Window.h"
#include "../Renderer/Renderer.h"
#include <memory>
#include <chrono>

#include "../Assets/AssetManager.h"


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

        bool isEditor();

        Window& getWindow();
        Renderer& getRenderer();
        AssetManager& getAssetManager();
    private:
        void render();
        void update();
        void exit();
        static void onWindowClosed(WindowClosed& event);

        bool m_isRunning = false;
        bool m_isEditor = false;
        std::unique_ptr<Window> m_window;
        std::unique_ptr<Renderer> m_renderer;
        std::unique_ptr<AssetManager> m_assetManager;
        std::shared_ptr<IGameInstance> m_game;
        std::chrono::time_point<std::chrono::steady_clock> m_currentFrameTime;
    };
}
