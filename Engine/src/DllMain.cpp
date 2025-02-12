#include <windows.h>
#include "Engine.h"
#include "Logger.h"

BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    std::shared_ptr<Kita::Engine> engine = nullptr;

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            engine = Kita::Engine::getEngine();
            Kita::Logger::getEngineLogger()->info("Engine dll loaded");
            break;

        case DLL_PROCESS_DETACH:
            Kita::Engine::exit();
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}
