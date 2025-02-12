#pragma once

#include <memory>
#include "DllTemplate.h"
#include "IGameInstance.h"

namespace Kita {
    class KITAENGINE_API Engine {
    public:
        Engine();
        virtual ~Engine();
        static std::shared_ptr<Engine>& getEngine();
        static void loadGameInstance(std::shared_ptr<IGameInstance> instance);
        static void run();
        static void exit();

    private:
        static bool isRunning;
        static std::shared_ptr<Engine> engineInstance;
        static std::shared_ptr<IGameInstance> gameInstance;
    };
}
