#pragma once
#include "DllTemplate.h"
#include "../instances/IGameInstance.h"
#include "../Window/Window.h"
#include "../Renderer/Renderer.h"
#include <memory>
#include <chrono>

#include "../Assets/AssetManager.h"
#include "../Physics/PhysicsManager.h"
#include "../Threading/ThreadPool.h"


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

        bool isEditor() const;
        bool isFirstFrame() const;

        Window& getWindow() const;
        Renderer& getRenderer() const;
        AssetManager& getAssetManager() const;
        PhysicsManager& getPhysicsManager() const;
        ThreadPool& getThreadPool() const;
    private:
        void render();
        void update();
        void exit();
        static void onWindowClosed(WindowClosed& event);

        bool m_isRunning = false;
        bool m_isEditor = false;
        bool m_isFirstFrame = true;
        std::unique_ptr<Window> m_window;
        std::unique_ptr<Renderer> m_renderer;
        std::unique_ptr<AssetManager> m_assetManager;
        std::unique_ptr<PhysicsManager> m_physicsManager;
        std::unique_ptr<ThreadPool> m_threadPool;
        std::shared_ptr<IGameInstance> m_game;
        std::chrono::time_point<std::chrono::steady_clock> m_currentFrameTime;
    };
}
