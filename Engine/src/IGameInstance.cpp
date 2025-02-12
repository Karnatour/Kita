#include "IGameInstance.h"

#include "Engine.h"

namespace Kita {
    extern "C" KITAENGINE_API void registerGameInstance(IGameInstance* instance) {
        Engine::loadGameInstance(std::shared_ptr<IGameInstance>(instance));
    }
}
