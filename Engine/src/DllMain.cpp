#include "kitapch.h"
#include "Engine.h"

BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
#ifdef KITA_BUILD_DEBUG
            std::cout << "Kita Dll attached" << std::endl;
#endif
            Kita::Logger::init();
            Kita::Engine::init();
            break;

        case DLL_PROCESS_DETACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}
