#pragma once
#include <memory>
#include "KitaEngine/Kita.h"

#ifdef SANDBOX_BUILD_DLL_EXPORTS
#define SANDBOX_API __declspec(dllexport)
#else
    #define SANDBOX_API __declspec(dllimport)
#endif

class SANDBOX_API Sandbox final : public Kita::IGameInstance {
public:
    void onInit() override;
    void onUpdate() override;
    void onRender() override;
    void onExit() override;

private:
    std::unique_ptr<Kita::Scene> m_scene;
};

extern "C" SANDBOX_API Kita::IGameInstance* createGameInstance();

