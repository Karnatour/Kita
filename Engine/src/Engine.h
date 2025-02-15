#pragma once

#include <memory>
#include "DllTemplate.h"
#include "IGameInstance.h"

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
