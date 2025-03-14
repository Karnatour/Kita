#include "../kitapch.h"
#include "EngineListeners.h"
#include "../Engine.h"

namespace Kita {
    void EngineListeners::closeEngine(WindowClosed& event) {
        Engine::getEngine()->stop();
    }
}
