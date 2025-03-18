#include "IGameInstance.h"
#include "../Core/Engine.h"

namespace Kita {
    extern "C" KITAENGINE_API void registerGameInstance(IGameInstance* instance) {
        Engine::getEngine()->loadGameInstance(std::shared_ptr<IGameInstance>(instance));
    }
}