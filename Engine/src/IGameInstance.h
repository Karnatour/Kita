#pragma once

#include "DllTemplate.h"

class IGameInstance {
public:
    virtual void onUpdate() = 0;
    virtual void onRender() = 0;
    virtual ~IGameInstance() = default;
};

extern "C" KITAENGINE_API void registerGameInstance(IGameInstance* instance);
