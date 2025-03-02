#pragma once
#include "IGameInstance.h"
#include "DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Engine {
    public:
        Engine();
        static void init();
        static std::shared_ptr<Engine>& getEngine();

        void loadGameInstance(std::shared_ptr<IGameInstance> instance);
        void run();
        void exit();

    private:
        bool m_isRunning = false;
        std::shared_ptr<IGameInstance> m_game = nullptr;
    };
}
