#pragma once
#include "DllTemplate.h"

class IGameInstance {
public:
    virtual void onInit() = 0;
    virtual void onUpdate() = 0;
    virtual void onRender() = 0;
    virtual void onExit() = 0;
    virtual ~IGameInstance() = default;
    bool m_initialized = false;
};

extern "C" KITAENGINE_API void registerGameInstance(IGameInstance* instance);
