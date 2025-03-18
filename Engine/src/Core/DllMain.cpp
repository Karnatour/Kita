BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
#ifdef KITA_BUILD_DEBUG
            std::cout << "Kita Dll attached" << std::endl;
#endif
            Kita::Logger::init();
            break;
        case DLL_PROCESS_DETACH:
#ifdef KITA_BUILD_DEBUG
            std::cout << "Kita Dll deattached" << std::endl;
#endif
            break;
        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}
