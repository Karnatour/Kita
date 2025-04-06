#pragma once
#include "WindowEvents.h"

namespace Kita {
    struct EngineListeners {
        static void closeEngine(WindowClosed& event);
    };
}
