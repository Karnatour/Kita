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
    Kita::Scene& getScene();
private:
    void onKeyPressed(const Kita::KeyPressed& event);
    std::unique_ptr<Kita::Scene> m_scene;
    Kita::Entity m_sphere;
    Kita::Entity m_player;
    Kita::Entity m_vehicle;
};

extern "C" SANDBOX_API Kita::IGameInstance* createGameInstance();

