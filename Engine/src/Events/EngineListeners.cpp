#include "EngineListeners.h"
#include "../Core/Engine.h"

namespace Kita {
    void EngineListeners::closeEngine(WindowClosed& event) {
        Engine::getEngine()->stop();
    }
}
